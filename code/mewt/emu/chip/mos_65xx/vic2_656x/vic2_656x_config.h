
#pragma once

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.decl.h"
#include "mewt/gfx/video_output.h"

namespace mewt::emu::chip::mos_65xx {

	// mwToDo: Get rid of this and replace with virtual implementation overrides instead.

	struct vic2_config_t {

		// Video signal standard (PAL/NTSC) supported by this model of the Vic-II.
		gfx::video_output::standard_t _video_standard;

		// Number of CPU cycles on each scanline. The Vic-II will generate 8 pixels per CPU cycle.
		uint16_t _cycles_per_scanline;

		// Total number of scanlines per frane.
		uint16_t _total_scanlines_per_frame;

		// Value of RASTER when video output is switched on - this is the top of the visible display.
		uint16_t _raster_display_on;

		// Value of RASTER when video output is switched off - this is the bottom of the visible display.
		uint16_t _raster_display_off;

		// X-position of the raster beam in a scanline when the raster interrupt is fired.
		uint16_t _xpos_raster_irq;

		// X-position of the raster beam in a scanline when the display is turned on. This is the left of the visible display.
		uint16_t _xpos_display_on;

		// X-position of the raster beam in a scanline when the display is turned off. This is the right of the visible display.
		uint16_t _xpos_display_off;

		// Total number of pixels across an entire scanline.
		constexpr uint16_t total_pixels_per_scanline() const { return _cycles_per_scanline << 3; }

		// Number of scanlines in the visible portion of the display.
		constexpr uint16_t visible_scanline_count() const { return (_raster_display_off + _total_scanlines_per_frame - _raster_display_on) % _total_scanlines_per_frame; }

		// Number of pixels across a scanline in the visible portion of the display.
		constexpr uint16_t visible_scanline_width() const { return (_xpos_display_off + total_pixels_per_scanline() - _xpos_display_on) % total_pixels_per_scanline(); }

		// Get the config for a particular Vic-II model.
		//static const vic2_config_t& get(vic2_model_t model);

	};

}
