
#pragma once

#include <cstdint>

namespace mewt::types {

	struct ColourT
	{
		inline ColourT() = default;
		inline ColourT(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
			 : b(b), g(g), r(r), a(a)
		{
		}
		uint8_t b, g, r, a;
	};
}
