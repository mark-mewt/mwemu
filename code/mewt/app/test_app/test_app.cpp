
#include "mewt/app/test_app/test_app.h"
#include "mewt/app_type/realtime/realtime_app_phase.impl.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/async/future_promise.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/types/scale_factor.h"
#include "mewt/ext/sdl/sdl_event_manager.impl.h"

#include "mewt/gfx/image.h"
// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

#include <span>

namespace mewt::app::test_app
{

	// mwToDo: Remove these...

	template <typename TType>
	auto componentwiseSubtract(TType lhs, TType rhs)
	{
		TType ret;
		TType::withComponents([&](auto cmp) { ret.*cmp = lhs.*cmp - rhs.*cmp; });
		return ret;
	}

	template <typename TType>
	auto componentwiseScale(TType lhs, types::scale_factor_t scale)
	{
		TType ret;
		TType::withComponents([&](auto cmp) { ret.*cmp = scale.scale(lhs.*cmp); });
		return ret;
	}

	constexpr auto sizeToPosition(ext::sdl::image_t::Size size)
		 -> ext::sdl::image_t::Position
	{
		return ext::sdl::image_t::Position{ ._x = ext::sdl::image_t::XPosition(size._width.get()), ._y = ext::sdl::image_t::YPosition(size._height.get()) };
	}

	enum class Keypress
	{
		Up,
		Down,
		Left,
		Right
	};

	struct TestApp::SharedState
	{
		gfx::Image::Rect _rect;
		gfx::Image::Rect _output_bounds;
		using Keypresses = types::flags<Keypress>;
		Keypresses _keypresses{ 0 };
	};

	void TestApp::initApp()
	{
		runUpdateLoop();
		runInputLoop();
		runRenderer();
	}

	auto TestApp::runRenderer()
		 -> async::Future<>
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

		SharedState sharedState;
		_shared_state = std::addressof(sharedState);

		// let us control our image position
		// so that we can move it with our keyboard.
		auto texture_size = sdl_texture.get_config()._size;
		_shared_state->_output_bounds = init_state.renderer().get_output_bounds();
		_shared_state->_rect = {
			._position = sizeToPosition(componentwiseScale(componentwiseSubtract(_shared_state->_output_bounds.size, texture_size), types::scale_factor_t::Half())),
			.size = texture_size
		};

		for (;;)
		{
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
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = _shared_state->_rect });
		}
	}

	auto TestApp::runInputLoop()
		 -> async::Future<>
	{
		for (;;)
		{
			auto input_event = co_await eventManager().keyboardEvent();
			bool pressed = false;
			switch (input_event.eventType())
			{

			case ext::sdl::KeyboardEvent::EventType::KeyDown:
				pressed = true;
			case ext::sdl::KeyboardEvent::EventType::KeyUp:
				// keyboard API for key pressed
				switch (input_event.scancode())
				{
				case ext::sdl::keyboard::Scancode::W:
				case ext::sdl::keyboard::Scancode::Up:
					_shared_state->_keypresses[Keypress::Up] = pressed;
					break;
				case ext::sdl::keyboard::Scancode::A:
				case ext::sdl::keyboard::Scancode::Left:
					_shared_state->_keypresses[Keypress::Left] = pressed;
					break;
				case ext::sdl::keyboard::Scancode::S:
				case ext::sdl::keyboard::Scancode::Down:
					_shared_state->_keypresses[Keypress::Down] = pressed;
					break;
				case ext::sdl::keyboard::Scancode::D:
				case ext::sdl::keyboard::Scancode::Right:
					_shared_state->_keypresses[Keypress::Right] = pressed;
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	auto TestApp::runUpdateLoop()
		 -> async::Future<>
	{
		using ext::sdl::image_t;
		// speed of box
		const int box_speed = 10;
		image_t::Size frame_delta = { // mwToDo: we can have a speed metric, (size/time_unit), then we can multiply by time unit to get the frame delta
												._width = image_t::Width(box_speed),
												._height = image_t::Height(box_speed)
		};
		for (;;)
		{
			co_await phaseManager().phase<PhaseType::Update>();
			if (_shared_state->_keypresses[Keypress::Up])
				_shared_state->_rect._position._y -= frame_delta._height;
			if (_shared_state->_keypresses[Keypress::Left])
				_shared_state->_rect._position._x -= frame_delta._width;
			if (_shared_state->_keypresses[Keypress::Down])
				_shared_state->_rect._position._y += frame_delta._height;
			if (_shared_state->_keypresses[Keypress::Right])
				_shared_state->_rect._position._x += frame_delta._width;
			if (right(_shared_state->_rect) > right(_shared_state->_output_bounds))
				_shared_state->_rect._position._x = right(_shared_state->_output_bounds) - _shared_state->_rect.size._width; // mwToDo: dest.right() = output_bounds.right() - dest.width()

			// left boundary
			if (left(_shared_state->_rect) < left(_shared_state->_output_bounds))
				_shared_state->_rect._position._x = left(_shared_state->_output_bounds);

			// bottom boundary
			if (bottom(_shared_state->_rect) > bottom(_shared_state->_output_bounds))
				_shared_state->_rect._position._y = bottom(_shared_state->_output_bounds) - _shared_state->_rect.size._height;

			// upper boundary
			if (top(_shared_state->_rect) < top(_shared_state->_output_bounds))
				_shared_state->_rect._position._y = top(_shared_state->_output_bounds);
		}
	}

}
