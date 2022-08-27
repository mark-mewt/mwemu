
#include "mewt/app_type/realtime/realtime_app_state.h"

namespace mewt::app_type::realtime {

	realtime_app_t::state_t::state_t()
		 : _sdl_engine(ext::sdl::subsystem_t::Video | ext::sdl::subsystem_t::Events),

			_sdl_window("Mewt Emulator",
							ext::sdl::window_t::centered_t{},
							ext::sdl::window_t::centered_t{},
							ext::sdl::window_t::width_t(1000),
							ext::sdl::window_t::height_t(1000), types::no_flags_t{}),

			_sdl_renderer(_sdl_window, ext::sdl::renderer_t::FirstDriver, ext::sdl::renderer_t::flag_t::Accelerated | ext::sdl::renderer_t::flag_t::PresentVSync) {
	}

}