
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r56a.h"

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"

namespace mewt::emu::chip::mos_65xx
{

	auto Vic26567r56aT::getConfig() const -> const vic2_config_t&
	{
		static constexpr vic2_config_t kConfig = {
			._video_standard = gfx::video_output::Standard::NtscM,
			._cycles_per_scanline = 64,
			._total_scanlines_per_frame = 262,
			._raster_display_on = 41,
			._raster_display_off = 13,
			._xpos_raster_irq = 412,
			._xpos_display_on = 488,
			._xpos_display_off = 387, // 388,
		};
		return kConfig;
	}
}
