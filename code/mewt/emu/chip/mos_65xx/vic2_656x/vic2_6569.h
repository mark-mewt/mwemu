
#pragma once

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"

namespace mewt::emu::chip::mos_65xx {

	class vic2_6569_t : public vic2_656x_t {

	protected:
		const vic2_config_t& get_config() const override final;
	};

}
