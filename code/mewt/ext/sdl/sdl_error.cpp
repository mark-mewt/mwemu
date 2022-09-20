
#include "mewt/ext/sdl/sdl_error.h"

#include "mewt/diag/log.h"

#include <string>
#ifndef __cpp_consteval
#define __cpp_consteval // fix for clang
#endif
#include <source_location>

#include "SDL/SDL_error.h"

namespace mewt::ext::sdl {

	void throw_error(std::source_location loc) {
		auto* err = SDL_GetError();
		//logger(loc).log("SDL Error: %s", err);
		throw exception_t{ err };
	}

}
