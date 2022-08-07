
#pragma once

#include "mewt/diag/source_location.h"

namespace mewt::diag {

	class log_context_t {

	public:
		log_context_t(source_location_t loc);

		void log(const char* fmt, ...);

	private:
		source_location_t _loc;
	};

	log_context_t logger(source_location_t loc = source_location_t::current());

}

namespace mewt {
	using diag::logger;
}
