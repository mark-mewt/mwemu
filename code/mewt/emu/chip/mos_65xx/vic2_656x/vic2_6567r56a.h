
#pragma once

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"

namespace mewt::emu::chip::mos_65xx {

	class Vic26567r56aT : public vic2_656x_t
	{

	protected:
		auto getConfig() const -> const vic2_config_t& final;
	};
}
