
#pragma once

#include "mewt/types/byte.h"

namespace mewt::types
{

	struct Colour
	{
		uint8_t b, g, r, a;
	};

	constexpr auto colourFromRGB(std::uint32_t rgb)
	{
		Colour colour = {
			.b = lowByte(rgb),
			.g = secondByte(rgb),
			.r = thirdByte(rgb),
			.a = std::numeric_limits<uint8_t>::max(),
		};
		return colour;
	}

}
