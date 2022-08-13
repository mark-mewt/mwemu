
#include "mewt/emu/sys/c64/c64.h"

#include "mewt/os/app_context/app_context.h"
#include "mewt/gui/window_manager/window_manager.h"
#include "mewt/gui/window/window.h"
#include "mewt/ext/sdl/sdl_engine.h"
#include "mewt/ext/sdl/sdl_window.h"
#include "mewt/ext/sdl/sdl_renderer.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/types/scale_factor.h"
#include "mewt/ext/sdl/sdl_main_loop.h"

// mwToDo: Abstract these away
#include "SDL/SDL_events.h"
#include "SDL/SDL_render.h"
#include "SDL/SDL_timer.h"

namespace mewt {

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

	int mewt_main(const os::app_context_i& app_context) {

		namespace sdl = ext::sdl;

		sdl::engine_t sdl_engine(sdl::subsystem_t::Video | sdl::subsystem_t::Events);

		sdl::window_t sdl_window("Mewt Emulator",
										 sdl::window_t::centered_t{},
										 sdl::window_t::centered_t{},
										 sdl::window_t::width_t(1000),
										 sdl::window_t::height_t(1000), types::no_flags_t{});

		sdl::renderer_t sdl_renderer(sdl_window, sdl::renderer_t::FirstDriver, sdl::renderer_t::flag_t::Accelerated);

		sdl::texture_config_t texture_config{
			._format = sdl::pixel_format_t::coded<sdl::pixel_format_t::preset_t::ARGB8888>(),
			._access = sdl::texture_config_t::access_t::Streaming,
			._size = { ._width = sdl::image_t::width_t(32),
						  ._height = sdl::image_t::height_t(32) }
		};

		sdl::texture_t sdl_texture(sdl_renderer, texture_config);

		// let us control our image position
		// so that we can move it with our keyboard.
		auto texture_size = sdl_texture.get_config()._size;
		auto output_bounds = sdl_renderer.get_output_bounds();
		sdl::image_t::rect_t dest {
			._position = size_to_position(componentwise_scale(componentwise_subtract(output_bounds._size, texture_size), types::scale_factor_t::Half())),
			._size = texture_size
		};

		// controls animation loop
		int close = 0;

		// speed of box
		sdl::image_t::size_t frame_delta{ // mwToDo: we can have a speed metric, (size/time_unit), then we can multiply by time unit to get the frame delta
													 ._width = sdl::image_t::width_t(10),
													 ._height = sdl::image_t::height_t(10)
		};

		sdl::event_dispatch_t event_dispatch;

		sdl::main_loop_t main_loop(event_dispatch);
		for (auto& frame : main_loop) {
			(void)frame;
		}

		// animation loop
		while (!close) {
			SDL_Event event;

			// Events management
			while (SDL_PollEvent(&event)) {
				switch (event.type) {

				case SDL_QUIT:
					// handling of close button
					close = 1;
					break;

				case SDL_KEYDOWN:
					// keyboard API for key pressed
					switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_UP:
						dest._position._y -= frame_delta._height;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						dest._position._x -= frame_delta._width;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						dest._position._y += frame_delta._height;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						dest._position._x += frame_delta._width;
						break;
					default:
						break;
					}
				}
			}

			// right boundary
			if (dest.right() > output_bounds.right())
				dest._position._x = output_bounds.right() - dest._size._width;	// mwToDo: dest.right() = output_bounds.right() - dest.width()

			// left boundary
			if (dest.left() < output_bounds.left())
				dest._position._x = output_bounds.left();

			// bottom boundary
			if (dest.bottom() > output_bounds.bottom())
				dest._position._y = output_bounds.bottom() - dest._size._height;

			// upper boundary
			if (dest.top() < output_bounds.top())
				dest._position._y = output_bounds.top();

			// clears the screen
			sdl_renderer.clear_screen();

			char* pixels = 0;
			int pitch = 0;
			SDL_LockTexture(sdl_texture.get(), nullptr, (void**)&pixels, &pitch);
			static int kk = 0;
			kk++;
			for (int i = 0; i < 32 * 32; ++i)
				pixels[i] = i + kk;
			SDL_UnlockTexture(sdl_texture.get());

			//SDL_RenderCopy(sdl_renderer.get(), sdl_texture.get(), NULL, &dest);
			sdl_renderer.copy(sdl_texture, { ._src = std::nullopt, ._dest = dest });

			// triggers the double buffers
			// for multiple rendering
			sdl_renderer.present_screen();

			// calculates to 60 fps
			SDL_Delay(1000 / 60);
		}

		//gui::window_manager_t window_manager = gui::window_manager_i::create(app_context);
		//gui::window_t window = gui::window_i::create(window_manager);
		//window->run();

		//emu::sys::c64::c64_t c64;
		//c64.run_sys();
		return 0;
	}

}

/*
* 
* http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/
* 
* 
*/
