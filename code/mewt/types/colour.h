
#pragma once

#include <stdint.h>

namespace mewt::types {

	struct colour_t {
		inline colour_t() = default;
		inline colour_t(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
			 : r(r_), g(g_), b(b_), a(a_) {
		}
		uint8_t b, g, r, a;
	};

}
