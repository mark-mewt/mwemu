
#include "mewt/app/c64_emu/c64_emu_host.h"
#include "mewt/async/future_promise.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/types/scale_factor.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/app_type/realtime/realtime_app.impl.h"
#include "mewt/emu/sys/c64/c64_pal.h"
#include "mewt/emu/sys/c64/c64_ntsc.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

namespace mewt::app::c64_emu {

	c64_emu_host_t::c64_emu_host_t(app_type::realtime::realtime_app_t& app)
		 : _app(app) {
	}

	void c64_emu_host_t::init_host() {
		run_updater();
		run_input();
		run_renderer();
		int i = 0;
		if (i == 0)
			_c64 = std::make_unique<emu::sys::c64::c64_pal_t>();
		else if (i == 1)
			_c64 = std::make_unique<emu::sys::c64::c64_ntsc_t>();
		_c64->init_sys(*this);
	}

	async::future<> c64_emu_host_t::run_renderer() {

		auto& init_state = co_await _app.init_phase();

		using namespace ext::sdl;
		texture_config_t texture_config{
			._format = pixel_format_t::coded<pixel_format_t::preset_t::ARGB8888>(),
			._access = texture_config_t::access_t::Streaming,
			._size = _host_config.display_size
		};

		texture_t sdl_texture(init_state.renderer(), texture_config);

		auto output_bounds = init_state.renderer().get_output_bounds();

		for (;;) {
			co_await _app.update_phase();
			types::colour_t* pixels = nullptr;
			int pitch = 0;
			SDL_LockTexture(sdl_texture.get(), nullptr, (void**)&pixels, &pitch);
			auto span = types::span_2d_t(pixels, _host_config.display_size._width, _host_config.display_size._height, gfx::image_t::width_t(pitch / (int)sizeof(types::colour_t)));
			//static int kk = 0;
			//kk++;
			//for (int i = 0; i < 32 * 32; ++i)
			//	pixels[i] = i + kk;
			frame_t frame{ ._pixels = span };
			events.need_frame.dispatch(frame);
			SDL_UnlockTexture(sdl_texture.get());
			auto& render_data = co_await _app.render_phase();
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = output_bounds });
		}
	}

	async::future<> c64_emu_host_t::run_input() {
		for (;;) {
			auto input_event = co_await _app.event_manager().keyboard_event();
			switch (input_event.event_type()) {
			case ext::sdl::keyboard_event_t::event_type_t::KeyDown:
			case ext::sdl::keyboard_event_t::event_type_t::KeyUp:
				break;
			}
		}
	}

	async::future<> c64_emu_host_t::run_updater() {
		using namespace ext::sdl;
		for (;;) {
			co_await _app.update_phase();
		}
	}

}
