
#pragma once

#include <stdint.h>

namespace mewt::gfx::video_output {

	enum class standard_t {
		NTSC_M,
		PAL_B
	};

	struct config_t {
		uint8_t refresh_rate_hz;
		static config_t get(standard_t standard);
	};

}
