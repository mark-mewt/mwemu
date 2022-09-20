
#pragma once

#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.decl.h"
#include "mewt/gfx/video_output.h"

namespace mewt::emu::chip::mos_65xx {

	// mwToDo: Get rid of this and replace with virtual implementation overrides instead.

	struct vic2_config_t
	{

		// Video signal standard (PAL/NTSC) supported by this model of the Vic-II.
		// 6567: NTSC-M
		// 6569: PAL-B
		gfx::video_output::StandardT _video_standard;

		// Number of CPU cycles on each scanline. The Vic-II will generate 8 pixels per CPU cycle.
		// 6567R56A: 64 (0x40)
		// 6567R8: 65 (0x41)
		// 6569: 63 (0x3f)
		uint16_t _cycles_per_scanline;

		// Total number of scanlines per frane.
		// 6567R56A: 262 (0x106)
		// 6567R8: 263 (0x107)
		// 6569: 312 (0x138)
		uint16_t _total_scanlines_per_frame;

		// Value of RASTER when video output is switched on - this is the top of the visible display.
		// 6567: 41 (0x29)
		// 6569: 16 (0x10)
		uint16_t _raster_display_on;

		// Value of RASTER when video output is switched off - this is the bottom of the visible display.
		// 6567: 13 (0xd)
		// 6569: 300 (0x12c)
		uint16_t _raster_display_off;

		// X-position of the raster beam in a scanline when the raster interrupt is fired.
		// 6567: 412 (0x19c)
		// 6569: 404 (0x194)
		uint16_t _xpos_raster_irq;

		// X-position of the raster beam in a scanline when the display is turned on. This is the left of the visible display.
		// 6567R56A: 488 (0x1e8)
		// 6567R8: 489 (0x1e9)
		// 6569: 480 (0x1e0)
		uint16_t _xpos_display_on;

		// X-position of the raster beam in a scanline when the display is turned off. This is the right of the visible display.
		// 6567R56A: 388 (0x184)
		// 6567R8: 396 (0x18c)
		// 6569: 380 (0x17c)
		uint16_t _xpos_display_off;

		// Total number of pixels across an entire scanline.
		[[nodiscard]] constexpr auto totalPixelsPerScanline() const -> uint16_t { return _cycles_per_scanline << 3; }

		// Number of scanlines in the visible portion of the display.
		[[nodiscard]] constexpr auto visibleScanlineCount() const -> uint16_t { return (_raster_display_off + _total_scanlines_per_frame - _raster_display_on) % _total_scanlines_per_frame; }

		// Number of pixels across a scanline in the visible portion of the display.
		[[nodiscard]] constexpr auto visibleScanlineWidth() const -> uint16_t { return (_xpos_display_off + totalPixelsPerScanline() - _xpos_display_on) % totalPixelsPerScanline(); }

		// Get the config for a particular Vic-II model.
		//static const vic2_config_t& get(vic2_model_t model);

	};

}
