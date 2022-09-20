
#include "mewt/app/c64_emu/c64_emu.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/async/future_promise.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/types/scale_factor.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

namespace mewt::app::c64_emu {

	void c64_emu_t::init_app() {
		_host.runEmuHost();
	}

}
