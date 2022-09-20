
#include "mewt/app/test_app/test_app.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/async/future_promise.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/types/scale_factor.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

#include <span>

namespace mewt::app::test_app {

		// mwToDo: Remove these...

	template <typename TType>
	auto componentwiseSubtract(TType lhs, TType rhs) {
		TType ret;
		TType::withComponents([&](auto cmp) { ret.*cmp = lhs.*cmp - rhs.*cmp; });
		return ret;
	}

	template <typename TType>
	auto componentwiseScale(TType lhs, types::scale_factor_t scale) {
		TType ret;
		TType::withComponents([&](auto cmp) { ret.*cmp = scale.scale(lhs.*cmp); });
		return ret;
	}

	constexpr auto sizeToPosition(ext::sdl::image_t::Size size)
		 -> ext::sdl::image_t::Position
	{
		return ext::sdl::image_t::Position{ ._x = ext::sdl::image_t::XPosition(size._width.get()), ._y = ext::sdl::image_t::YPosition(size._height.get()) };
	}


	void TestApp::initApp()
	{
		runUpdateLoop();
		runInputLoop();
		runRenderer();
	}

	auto TestApp::runRenderer()
		 -> async::future<>
	{

		const auto& init_state = co_await phaseManager().phase<PhaseType::Init>();

		const int test_image_size = 32;
		const int test_pixel_count = test_image_size * test_image_size;

		using ext::sdl::texture_config_t;
		using ext::sdl::pixel_format_t;
		using ext::sdl::texture_t;
		using ext::sdl::image_t;
		texture_config_t texture_config{
			._format = pixel_format_t::coded<pixel_format_t::preset_t::ARGB8888>(),
			._access = texture_config_t::access_t::Streaming,
			._size = { ._width = image_t::Width(test_image_size),
						  ._height = image_t::Height(test_image_size) }
		};

		texture_t sdl_texture(init_state.renderer(), texture_config);

		// let us control our image position
		// so that we can move it with our keyboard.
		auto texture_size = sdl_texture.get_config()._size;
		_output_bounds = init_state.renderer().get_output_bounds();
		_rect = {
			._position = sizeToPosition(componentwiseScale(componentwiseSubtract(_output_bounds._size, texture_size), types::scale_factor_t::Half())),
			._size = texture_size
		};

		for (;;) {
			co_await phaseManager().phase<PhaseType::Update>();
			// mwToDo: This needs to move out of here. Ultimately, it will be the emulated GPU that does this.
			void* pixel_data = nullptr;
			int pitch = 0;
			SDL_LockTexture(sdl_texture.get(), nullptr, &pixel_data, &pitch);
			auto pixels = std::span(static_cast<unsigned char*>(pixel_data), test_pixel_count);
			static int rolling_offset = 0;
			rolling_offset++;
			for (int i = 0; i < test_pixel_count; ++i)
				pixels[i] = i + rolling_offset;
			SDL_UnlockTexture(sdl_texture.get());
			const auto& render_data = co_await phaseManager().phase<PhaseType::Render>();
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = _rect });
		}
	}

	auto TestApp::runInputLoop()
		 -> async::future<>
	{
		for (;;) {
			auto input_event = co_await eventManager().keyboard_event();
			bool pressed = false;
			switch (input_event.event_type()) {

			case ext::sdl::keyboard_event_t::event_type_t::KeyDown:
				pressed = true;
			case ext::sdl::keyboard_event_t::event_type_t::KeyUp:
				// keyboard API for key pressed
				switch (input_event.scancode()) {
				case ext::sdl::keyboard::scancode_t::W:
				case ext::sdl::keyboard::scancode_t::Up:
					_keypresses[Keypress::Up] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::A:
				case ext::sdl::keyboard::scancode_t::Left:
					_keypresses[Keypress::Left] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::S:
				case ext::sdl::keyboard::scancode_t::Down:
					_keypresses[Keypress::Down] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::D:
				case ext::sdl::keyboard::scancode_t::Right:
					_keypresses[Keypress::Right] = pressed;
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	auto TestApp::runUpdateLoop()
		 -> async::future<>
	{
		using ext::sdl::image_t;
		// speed of box
		const int box_speed = 10;
		image_t::Size frame_delta = { // mwToDo: we can have a speed metric, (size/time_unit), then we can multiply by time unit to get the frame delta
												._width = image_t::Width(box_speed),
												._height = image_t::Height(box_speed)
		};
		for (;;) {
			co_await phaseManager().phase<PhaseType::Update>();
			if (_keypresses[Keypress::Up])
				_rect._position._y -= frame_delta._height;
			if (_keypresses[Keypress::Left])
				_rect._position._x -= frame_delta._width;
			if (_keypresses[Keypress::Down])
				_rect._position._y += frame_delta._height;
			if (_keypresses[Keypress::Right])
				_rect._position._x += frame_delta._width;
			if (_rect.right() > _output_bounds.right())
				_rect._position._x = _output_bounds.right() - _rect._size._width; // mwToDo: dest.right() = output_bounds.right() - dest.width()

			// left boundary
			if (_rect.left() < _output_bounds.left())
				_rect._position._x = _output_bounds.left();

			// bottom boundary
			if (_rect.bottom() > _output_bounds.bottom())
				_rect._position._y = _output_bounds.bottom() - _rect._size._height;

			// upper boundary
			if (_rect.top() < _output_bounds.top())
				_rect._position._y = _output_bounds.top();
		}
	}

}
