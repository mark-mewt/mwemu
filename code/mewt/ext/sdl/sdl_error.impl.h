
#pragma once

#include "mewt/diag/source_location.h"

namespace mewt::ext::sdl {

	void throw_error(diag::source_location_t loc = diag::source_location_t::current());

	inline void check_status(int status, diag::source_location_t loc = diag::source_location_t::current()) {
		if (status != 0)
			throw_error(loc);
	}

	inline auto* check_pointer(auto* object, diag::source_location_t loc = diag::source_location_t::current()) {
		if (object == nullptr)
			throw_error(loc);
		return object;
	}

}
