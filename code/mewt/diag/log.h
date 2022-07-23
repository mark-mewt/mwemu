
#pragma once

#include <source_location>

namespace mewt::diag
{

	class log_context_t
	{

	public:

		log_context_t(std::source_location loc);

		void log(const char* fmt, ...);

	private:

		std::source_location _loc;

	};

	log_context_t logger(std::source_location loc = std::source_location::current());

}

namespace mewt { using diag::logger; }
