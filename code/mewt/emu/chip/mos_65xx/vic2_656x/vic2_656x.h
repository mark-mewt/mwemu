
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/types/flags.h"
#include "mewt/types/bitfield.h"

namespace mewt::emu::chip::mos_65xx
{

	class vic2_656x_t
   {
   private:
      const clock_source_t& _clock;
   public:

		struct io_controller_t : public memory_interface_t
		{
			vic2_656x_t& _chip;
			io_controller_t(vic2_656x_t& chip) : _chip(chip) {}
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		io_controller_t _io_controller{ *this };

		vic2_656x_t(const clock_source_t& clock);
      async::future<> run_gpu();

	private:

      async::future<> read_mem();
		async::future<> run_frame();
		async::future<> run_scanline(uint16_t raster_y);

		struct sprite_pos_t {
			uint8_t _x;
			uint8_t _y;
		};
		struct control_reg_1_t {
			int8_t _y_scroll : 3;
			bool _rsel : 1;
			bool _den : 1;
			bool _bmm : 1;
			bool _emm : 1;
			bool _rst8 : 1;
		};
		struct control_reg_2_t {
			int8_t _x_scroll : 3;
			bool _csel : 1;
			bool _mcm : 1;
			bool _res : 1;
		};
		struct memory_pointers_t {
			uint8_t _cb1 : 4;	// it appears the low bit of this cannot be written and is always 1 on reading
			uint8_t _vm1 : 4;
		};

		enum interrupt_t {
			RST,
			MBC,
			MMC,
			LP,
			IRQ = 7
		};

		class uint4_t {
			uint8_t _value;

		public:
			inline uint4_t(uint8_t v = 0) : _value(v) {}
			inline operator uint8_t() const { return _value | 0xf0; }
		};

		struct regs_t {
			// https://www.c64-wiki.com/wiki/Page_208-211
			sprite_pos_t _sprite_pos[8];
			types::bitfield<8> _sprite_x_msbs;
			control_reg_1_t _control_reg_1;
			uint8_t _raster;
			uint8_t _lightpen_x;
			uint8_t _lightpen_y;
			types::bitfield<8> _sprite_enabled;
			control_reg_2_t _control_reg_2;
			types::bitfield<8> _sprite_y_expand;
			memory_pointers_t _memory_pointers;
			types::flags<interrupt_t, 8> _interrupt_register{ 0 };
			types::flags<interrupt_t, 8> _interrupt_enabled{ 0 };
			types::bitfield<8> _sprite_data_priority;
			types::bitfield<8> _sprite_multicolor;
			types::bitfield<8> _sprite_x_expand;
			types::bitfield<8> _sprite_sprite_collision;
			types::bitfield<8> _sprite_data_collision;
			uint4_t _border_color;
			uint4_t _background_color_0;
			uint4_t _background_color_1;
			uint4_t _background_color_2;
			uint4_t _background_color_3;
			uint4_t _sprite_multicolor_0;
			uint4_t _sprite_multicolor_1;
			uint4_t _sprite_0_color;
			uint4_t _sprite_1_color;
			uint4_t _sprite_2_color;
			uint4_t _sprite_3_color;
			uint4_t _sprite_4_color;
			uint4_t _sprite_5_color;
			uint4_t _sprite_6_color;
			uint4_t _sprite_7_color;
		};
		regs_t _regs;

	};

}
