
#include "mewt/ext/sdl/sdl_engine.h"

#include "mewt/ext/sdl/sdl_error.impl.h"

#include <string>
#include "SDL/SDL.h"

namespace mewt::ext::sdl {

	engine_t::engine_t(subsystems_t subsystems) {
		check_status(SDL_InitSubSystem(subsystems.rawBits()));
	}

	void engine_t::start_subsystems(subsystems_t subsystems) {
		check_status(SDL_InitSubSystem(subsystems.rawBits()));
	}

	void engine_t::stop_subsystems(subsystems_t subsystems) {
		SDL_QuitSubSystem(subsystems.rawBits());
	}

	subsystems_t engine_t::query_started_subsystems(subsystems_t query_mask) {
		return subsystems_t(SDL_WasInit(query_mask.rawBits()));
	}

	engine_t::~engine_t() {
		SDL_Quit();
	}

}
