
#pragma once

#include <stdint.h>

namespace mewt::gfx::video_output
{

	enum class Standard
	{
		NtscM,
		PalB
	};

	struct Config
	{
		uint8_t refresh_rate_hz;
		static auto get(Standard standard) -> Config;
	};
}
