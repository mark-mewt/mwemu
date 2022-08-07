
#include "mewt/ext/sdl/sdl_error.h"

#include "mewt/diag/log.h"

#include <string>
#include "SDL/SDL_error.h"

namespace mewt::ext::sdl {

	void throw_error(std::source_location loc) {
		auto* err = SDL_GetError();
		logger(loc).log("SDL Error: %s", err);
		throw exception_t{ err };
	}

}
