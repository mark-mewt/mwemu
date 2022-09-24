
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.decl.h"
#include "mewt/emu/host/host.h"
#include "mewt/gfx/image.h"
#include "mewt/types/bitfield.h"
#include "mewt/types/flags.h"

namespace mewt::emu::chip::mos_65xx
{

	// NTSC: 1022.7 kHz CPU - 6567 - 8.18 MHz Pixel Clock
	// PAL: 985.248 kHz CPU - 6569 - 7.88 MHz Pixel Clock

	// Each character is 8x8 pixels
	// 40x25 text characters are displayed
	// 320x200 or 160x200 pixels in bitmap modea

	// In the 6567, RASTER=0 occurs at some point in the lower border
	// In the 6569, RASTER=0 occurs at the top of the vertical blank area

	// LPX=0 occurs at some point in the left border

	/*

	template <vic2_model_t _Model>
	inline constexpr vic2_config_t vic2_config{};

	template <>
	inline constexpr vic2_config_t vic2_config<vic2_model_t::VIC2_6567R56A> = {
		._video_standard = video_standard_t::NTSC_M,
		._cycles_per_scanline = 64,
		._first_visible_scanline = 41,
		._visible_scanlines_per_frame = 234,
		._xpos_raster_irq = 412, // $19c
		._xpos_display_start = 488, // $1e8
		._visible_width = 411,
	};

	template <>
	inline constexpr vic2_config_t vic2_config<vic2_model_t::VIC2_6567R8> = {
		._video_standard = video_standard_t::NTSC_M,
		._cycles_per_scanline = 65,
		._first_visible_scanline = 41,
		._visible_scanlines_per_frame = 235,
		._xpos_raster_irq = 412, // $19c
		._xpos_display_start = 489, // $1e9
		._visible_width = 418,
	};

	template <>
	inline constexpr vic2_config_t vic2_config<vic2_model_t::VIC2_6569> = {
		._video_standard = video_standard_t::PAL_B,
		._cycles_per_scanline = 63,
		._first_visible_scanline = 16,
		._visible_scanlines_per_frame = 284,
		._xpos_raster_irq = 404, // $194
		._xpos_display_start = 480, // $1e0
		._visible_width = 403,
	};

	struct vic2_derived_info_t {
		uint16_t _scanlines_per_frame;
		uint16_t _first_vblank_scanline;
		uint16_t _last_vblank_scanline;
		uint16_t _xpos_display_last;
		constexpr vic2_derived_info_t(vic2_config_t config) {
			_scanlines_per_frame = config._visible_scanlines_per_frame + 28;
			_last_vblank_scanline = config._first_visible_scanline - 1;
			_first_vblank_scanline = (config._first_visible_scanline + config._visible_scanlines_per_frame) % _scanlines_per_frame;
			_xpos_display_last = ((config._xpos_display_start + config._visible_width) % (config._cycles_per_scanline * 8)) + 1; // ???
		}
	};

	template<vic2_model_t _Model>
	inline constexpr vic2_derived_info_t vic2_derived_info = vic2_config<_Model>;

	//constexpr int x = sizeof(vic2_derived_info<vic2_model_t::VIC2_6569>);
	static_assert(vic2_derived_info<vic2_model_t::VIC2_6567R56A>._first_vblank_scanline == 13);
	static_assert(vic2_derived_info<vic2_model_t::VIC2_6567R8>._first_vblank_scanline == 13);
	static_assert(vic2_derived_info<vic2_model_t::VIC2_6569>._first_vblank_scanline == 300);

	static_assert(vic2_derived_info<vic2_model_t::VIC2_6567R56A>._xpos_display_last == 388); // $184
	//static_assert(vic2_derived_info<vic2_model_t::VIC2_6567R8>._xpos_display_last == 396); // $18c	mwToDo: Vic appears to hold x-pos for ~1.5 cycles
	static_assert(vic2_derived_info<vic2_model_t::VIC2_6569>._xpos_display_last == 380); // $17c

	template <vic2_model_t _Model>
	constexpr int clock_speed = vic2_config<_Model>._cycles_per_scanline* vic2_derived_info<_Model>._scanlines_per_frame* video_refresh_rate<vic2_config<_Model>._video_standard>;

	constexpr int clock_6567R56A = clock_speed<vic2_model_t::VIC2_6567R56A>;
	constexpr int clock_6567R8 = clock_speed<vic2_model_t::VIC2_6567R8>;
	constexpr int clock_6569 = clock_speed<vic2_model_t::VIC2_6569>;
	*/

	class vic2_656x_t
   {
   private:
      //const clock_source_t& _clock;
		//vic2_model_t _model;
		//const vic2_config_t& _config;

   public:
		struct IoControllerT : public MemoryInterface
		{
			vic2_656x_t& _chip;
			explicit IoControllerT(vic2_656x_t& chip) : _chip(chip) {}
			Data read(Address address) final;
			void write(Address address, Data data) final;
		};

		IoControllerT _io_controller{ *this };

		//vic2_656x_t(const clock_source_t& clock);//, vic2_model_t model);
		auto runGpu(IHost& host) -> async::Future<>;

		//gfx::image_t::size_t display_size() const;

		inline auto cpuClock() const -> const chip::clock_source_t& { return _cpu_clock; }

	protected:
		virtual auto getConfig() const -> const vic2_config_t& = 0;

	private:
		auto readMem() -> async::Future<>;
		auto runFrame() -> async::Future<>;
		auto runScanline(uint16_t raster_y) -> async::Future<>;
		void generateFrame(IHost::Frame& frame);

		chip::clock_source_t _cpu_clock;

		struct SpritePosT
		{
			uint8_t _x;		// Lower 8-bits of sprite x-coord.
			uint8_t _y;		// Sprite y-coord.
		};

		// $d011:
		struct ControlReg1T
		{
			int8_t _y_scroll : 3;	// Fine y-scroll of display window.
			bool _rsel : 1;			// 0: 24 text lines/192 pixels, Raster lines 55-246; 1: 25 text lines/200 pixels, Raster lines 51-250
			bool _den : 1;				// Display ENable - 1 to enable display, 0 to display only border colour
			bool _bmm : 1;				// 0: Load character data; 1: Load bitmap data
			bool _ecm : 1;				// Extended colour mode
			bool _rst8 : 1;			// Most significant bit of raster-y position.
		};
		struct ControlReg2T
		{
			int8_t _x_scroll : 3;	// Fine x-scroll of display window.
			bool _csel : 1;			// 0: 38 characters/304 pixels wide, LPX 31-334; 1: 40 characters/320 pixels wide, LPX 24-343
			bool _mcm : 1;				// Multi-colour mode
			bool _res : 1;
		};
		struct MemoryPointersT
		{
			uint8_t _cb1 : 4;	// it appears the low bit of this cannot be written and is always 1 on reading
			uint8_t _vm1 : 4;
		};

		enum InterruptT : std::uint8_t
		{
			RST,
			MBC,
			MMC,
			LP,
			IRQ = 7
		};

		class Uint4T
		{
			uint8_t _value;

		public:
			constexpr Uint4T(uint8_t v = 0) : _value(v) {}
			constexpr explicit operator uint8_t() const { return _value & 0xf; }
		};

		struct RegsT
		{
			// https://www.c64-wiki.com/wiki/Page_208-211
			SpritePosT _sprite_pos[8];
			types::BitField<8> _sprite_x_msbs;	// $d010: Most significant bits of sprite x-coords
			ControlReg1T _control_reg_1;			// $d011:
			uint8_t _raster;							// $d012: Low 8-bits of raster y-position.
			uint8_t _lightpen_x;
			uint8_t _lightpen_y;
			types::BitField<8> _sprite_enabled;
			ControlReg2T _control_reg_2;
			types::BitField<8> _sprite_y_expand;
			MemoryPointersT _memory_pointers;
			types::flags<InterruptT> _interrupt_register{ 0 };
			types::flags<InterruptT> _interrupt_enabled{ 0 };
			types::BitField<8> _sprite_data_priority;
			types::BitField<8> _sprite_multicolor;
			types::BitField<8> _sprite_x_expand;
			types::BitField<8> _sprite_sprite_collision;
			types::BitField<8> _sprite_data_collision;
			Uint4T _border_color;
			Uint4T _background_color_0;
			Uint4T _background_color_1;
			Uint4T _background_color_2;
			Uint4T _background_color_3;
			Uint4T _sprite_multicolor_0;
			Uint4T _sprite_multicolor_1;
			Uint4T _sprite_0_color;
			Uint4T _sprite_1_color;
			Uint4T _sprite_2_color;
			Uint4T _sprite_3_color;
			Uint4T _sprite_4_color;
			Uint4T _sprite_5_color;
			Uint4T _sprite_6_color;
			Uint4T _sprite_7_color;
		};
		RegsT _regs{};

		inline auto borderLeftCompare() const -> uint16_t { return _regs._control_reg_2._csel ? 0x18 : 0x1f; }
		inline auto borderRightCompare() const -> uint16_t { return _regs._control_reg_2._csel ? 0x158 : 0x14f; }
		inline auto borderTopCompare() const -> uint16_t { return _regs._control_reg_1._rsel ? 0x33 : 0x37; }
		inline auto borderBottomCompare() const -> uint16_t { return _regs._control_reg_1._rsel ? 0xfb : 0xf7; }
	};

}
