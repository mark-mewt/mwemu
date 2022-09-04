
#pragma once

#include "mewt/emu/sys/c64/c64.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r8.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r56a.h"

namespace mewt::emu::sys::c64 {

	class c64_ntsc_t : public c64_t {

	protected:
		inline chip::mos_65xx::vic2_656x_t& get_vic2() override final {
			if (_use_r56a)
				return _vic2_6567r56a;
			else
				return _vic2_6567r8;
		}

	private:
		chip::mos_65xx::vic2_6567r56a_t _vic2_6567r56a;
		chip::mos_65xx::vic2_6567r8_t _vic2_6567r8;
		bool _use_r56a = false;

	};

}
