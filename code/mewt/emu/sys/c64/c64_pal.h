
#pragma once

#include "mewt/emu/sys/c64/c64.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6569.h"

namespace mewt::emu::sys::c64 {

	class c64_pal_t : public c64_t {

	public:
		inline c64_pal_t() : c64_t(_vic2) { }

	private:

		vic2_6569_t _vic2;

	};

}
