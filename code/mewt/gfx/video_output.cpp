
#include "mewt/gfx/video_output.h"

#include <exception>

namespace mewt::gfx::video_output {

	template <StandardT _Standard>
	struct standard_info;

	template <>
	struct standard_info<StandardT::NtscM> {
		static constexpr ConfigT config = {
			.refresh_rate_hz = 60,
		};
	};

	template <>
	struct standard_info<StandardT::PalB> {
		static constexpr ConfigT config = {
			.refresh_rate_hz = 50,
		};
	};

	ConfigT ConfigT::get(StandardT standard) {
		switch (standard) {
		case StandardT::NtscM:
			return standard_info<StandardT::NtscM>::config;
		case StandardT::PalB:
			return standard_info<StandardT::PalB>::config;
		}
		throw std::exception("Unknown standard.");
	}

}
