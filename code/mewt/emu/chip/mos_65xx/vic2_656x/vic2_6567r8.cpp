
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6567r8.h"

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"

namespace mewt::emu::chip::mos_65xx
{

	const vic2_config_t& vic2_6567r8_t::getConfig() const
	{
		static constexpr vic2_config_t config = {
			._video_standard = gfx::video_output::Standard::NtscM,
			._cycles_per_scanline = 65,
			._total_scanlines_per_frame = 263,
			._raster_display_on = 41,
			._raster_display_off = 13,
			._xpos_raster_irq = 412,
			._xpos_display_on = 489,
			._xpos_display_off = 387, // 396,
		};
		return config;
	}

}
