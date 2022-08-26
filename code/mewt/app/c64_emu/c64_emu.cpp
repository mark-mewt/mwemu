
#include "mewt/app/c64_emu/c64_emu.h"
#include "mewt/async/future_promise.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/types/scale_factor.h"
#include "mewt/ext/sdl/sdl_scancode.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

namespace mewt::app::c64_emu {

	void c64_emu_t::init_app(const init_state_t& init_state) {
		run_updater();
		run_input();
		run_renderer();
		_c64.run_sys();
	}

	async::future<> c64_emu_t::run_renderer() {

		auto& init_state = co_await phase_manager().phase<phase_manager_t::phase_type_t::Init>();

		using namespace ext::sdl;
		texture_config_t texture_config{
			._format = pixel_format_t::coded<pixel_format_t::preset_t::ARGB8888>(),
			._access = texture_config_t::access_t::Streaming,
			._size = { ._width = image_t::width_t(32),
						  ._height = image_t::height_t(32) }
		};

		texture_t sdl_texture(init_state.renderer(), texture_config);

		auto output_bounds = init_state.renderer().get_output_bounds();

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
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = output_bounds });
		}
	}

	async::future<> c64_emu_t::run_input() {
		for (;;) {
			auto input_event = co_await event_manager().keyboard_event();
			switch (input_event.event_type()) {
			case ext::sdl::keyboard_event_t::event_type_t::KeyDown:
			case ext::sdl::keyboard_event_t::event_type_t::KeyUp:
				break;
			}
		}
	}

	async::future<> c64_emu_t::run_updater() {
		using namespace ext::sdl;
		for (;;) {
			co_await phase_manager().phase<phase_manager_t::phase_type_t::Update>();
		}
	}

}
