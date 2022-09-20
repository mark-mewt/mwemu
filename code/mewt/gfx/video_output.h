
#pragma once

#include <stdint.h>

namespace mewt::gfx::video_output {

	enum class StandardT
	{
		NtscM,
		PalB
	};

	struct ConfigT
	{
		uint8_t refresh_rate_hz;
		static ConfigT get(StandardT standard);
	};
}
