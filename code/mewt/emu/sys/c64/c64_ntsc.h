
#pragma once

#include "mewt/emu/sys/c64/c64.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r8.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r56a.h"

namespace mewt::emu::sys::c64 {

	class c64_ntsc_t : public c64_t {

	public:
		inline c64_ntsc_t() : c64_t(_vic2) {}

	private:
		#if 0
		chip::mos_65xx::vic2_6567r56a_t _vic2;
		#else
		chip::mos_65xx::vic2_6567r8_t _vic2;
		#endif

	};

}
