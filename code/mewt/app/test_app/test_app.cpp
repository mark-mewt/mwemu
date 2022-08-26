
#include "mewt/app/test_app/test_app.h"
#include "mewt/async/future_promise.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/types/scale_factor.h"
#include "mewt/ext/sdl/sdl_scancode.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

namespace mewt::app::test_app {

		// mwToDo: Remove these...

	template <typename _Type>
	auto componentwise_subtract(_Type lhs, _Type rhs) {
		_Type ret;
		_Type::with_components([&](auto cmp) { ret.*cmp = lhs.*cmp - rhs.*cmp; });
		return ret;
	}

	template <typename _Type>
	auto componentwise_scale(_Type lhs, types::scale_factor_t scale) {
		_Type ret;
		_Type::with_components([&](auto cmp) { ret.*cmp = scale.scale(lhs.*cmp); });
		return ret;
	}

	constexpr ext::sdl::image_t::position_t size_to_position(ext::sdl::image_t::size_t s) {
		return ext::sdl::image_t::position_t{ ._x = ext::sdl::image_t::x_position_t(s._width.get()), ._y = ext::sdl::image_t::y_position_t(s._height.get()) };
	}


	void test_app_t::init_app(const init_state_t& init_state) {
		run_update_loop();
		run_input_loop();
		run_renderer();
	}

	async::future<> test_app_t::run_renderer() {

		auto& init_state = co_await phase_manager().phase<phase_manager_t::phase_type_t::Init>();

		using namespace ext::sdl;
		texture_config_t texture_config{
			._format = pixel_format_t::coded<pixel_format_t::preset_t::ARGB8888>(),
			._access = texture_config_t::access_t::Streaming,
			._size = { ._width = image_t::width_t(32),
						  ._height = image_t::height_t(32) }
		};

		texture_t sdl_texture(init_state.renderer(), texture_config);

		// let us control our image position
		// so that we can move it with our keyboard.
		auto texture_size = sdl_texture.get_config()._size;
		_output_bounds = init_state.renderer().get_output_bounds();
		_rect = {
			._position = size_to_position(componentwise_scale(componentwise_subtract(_output_bounds._size, texture_size), types::scale_factor_t::Half())),
			._size = texture_size
		};

		for (;;) {
			co_await phase_manager().phase<phase_manager_t::phase_type_t::Update>();
			// mwToDo: This needs to move out of here. Ultimately, it will be the emulated GPU that does this.
			char* pixels = 0;
			int pitch = 0;
			SDL_LockTexture(sdl_texture.get(), nullptr, (void**)&pixels, &pitch);
			static int kk = 0;
			kk++;
			for (int i = 0; i < 32 * 32; ++i)
				pixels[i] = i + kk;
			SDL_UnlockTexture(sdl_texture.get());
			auto& render_data = co_await phase_manager().phase<phase_manager_t::phase_type_t::Render>();
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = _rect });
		}
	}

	async::future<> test_app_t::run_input_loop() {
		for (;;) {
			auto input_event = co_await event_manager().keyboard_event();
			bool pressed = false;
			switch (input_event.event_type()) {

			case ext::sdl::keyboard_event_t::event_type_t::KeyDown:
				pressed = true;
			case ext::sdl::keyboard_event_t::event_type_t::KeyUp:
				// keyboard API for key pressed
				switch (input_event.scancode()) {
				case ext::sdl::keyboard::scancode_t::W:
				case ext::sdl::keyboard::scancode_t::Up:
					_keypresses[keypress_t::Up] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::A:
				case ext::sdl::keyboard::scancode_t::Left:
					_keypresses[keypress_t::Left] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::S:
				case ext::sdl::keyboard::scancode_t::Down:
					_keypresses[keypress_t::Down] = pressed;
					break;
				case ext::sdl::keyboard::scancode_t::D:
				case ext::sdl::keyboard::scancode_t::Right:
					_keypresses[keypress_t::Right] = pressed;
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	async::future<> test_app_t::run_update_loop() {
		using namespace ext::sdl;
		// speed of box
		image_t::size_t frame_delta = { // mwToDo: we can have a speed metric, (size/time_unit), then we can multiply by time unit to get the frame delta
												  ._width = image_t::width_t(10),
												  ._height = image_t::height_t(10)
		};
		for (;;) {
			co_await phase_manager().phase<phase_manager_t::phase_type_t::Update>();
			if (_keypresses[keypress_t::Up])
				_rect._position._y -= frame_delta._height;
			if (_keypresses[keypress_t::Left])
				_rect._position._x -= frame_delta._width;
			if (_keypresses[keypress_t::Down])
				_rect._position._y += frame_delta._height;
			if (_keypresses[keypress_t::Right])
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
