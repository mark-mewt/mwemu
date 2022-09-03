
#include "mewt/gfx/video_output.h"

#include <exception>

namespace mewt::gfx::video_output {

	template <standard_t _Standard>
	struct standard_info;

	template <>
	struct standard_info<standard_t::NTSC_M> {
		static constexpr config_t config = {
			.refresh_rate_hz = 60,
		};
	};

	template <>
	struct standard_info<standard_t::PAL_B> {
		static constexpr config_t config = {
			.refresh_rate_hz = 50,
		};
	};

	config_t config_t::get(standard_t standard) {
		switch (standard) {
		case standard_t::NTSC_M:
			return standard_info<standard_t::NTSC_M>::config;
		case standard_t::PAL_B:
			return standard_info<standard_t::PAL_B>::config;
		}
		throw std::exception("Unknown standard.");
	}

}
