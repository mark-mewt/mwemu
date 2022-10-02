
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_6569.h"

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"

namespace mewt::emu::chip::mos_65xx
{

	const vic2_config_t& vic2_6569_t::getConfig() const
	{
		static constexpr vic2_config_t config = {
			._video_standard = gfx::video_output::Standard::NtscM,
			._cycles_per_scanline = 63,
			._total_scanlines_per_frame = 312,
			._raster_display_on = 16,
			._raster_display_off = 300,
			._xpos_raster_irq = 404,
			._xpos_display_on = 480,
			._xpos_display_off = 379, // 380,
		};
		return config;
	}

}
