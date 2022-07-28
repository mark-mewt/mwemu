
#pragma once

#include "async/awaitable_func.decl.h"
#include "emu/clock.decl.h"
#include "emu/bus_spec.h"
#include "emu/memory_interface.h"

namespace mewt::emu::gpu
{

   class vic2_t
   {
   private:
      const clock_source_t& _clock;
   public:

		using bus_spec_t = bus_spec<16, 8>;
		using address_t = bus_spec_t::address_t;
		using data_t = bus_spec_t::data_t;

		struct io_controller_t : public memory_interface<bus_spec_t>
		{
			vic2_t& _sys;
			struct sprite_pos_t
			{
				data_t _x;
				data_t _y;
			};
			struct regs_t
			{
				// https://www.c64-wiki.com/wiki/Page_208-211
				sprite_pos_t _sprite_pos[8];
				data_t _sprite_x_msbs;
				data_t _control_reg_1;
				data_t _raster;
				data_t _lightpen_x;
				data_t _lightpen_y;
				data_t _sprite_enabled;
				data_t _control_reg_2;
				data_t _sprite_y_expand;
				data_t _memory_pointers;
				data_t _interrupt_register;
				data_t _interrupt_enabled;
				data_t _sprite_data_priority;
				data_t _sprite_multicolor;
				data_t _sprite_x_expand;
				data_t _sprite_sprite_collision;
				data_t _sprite_data_collision;
				data_t _border_color;
				data_t _background_color_0;
				data_t _background_color_1;
				data_t _background_color_2;
				data_t _background_color_3;
				data_t _sprite_multicolor_0;
				data_t _sprite_multicolor_1;
				data_t _sprite_0_color;
				data_t _sprite_1_color;
				data_t _sprite_2_color;
				data_t _sprite_3_color;
				data_t _sprite_4_color;
				data_t _sprite_5_color;
				data_t _sprite_6_color;
				data_t _sprite_7_color;
			};
			regs_t _regs;
			io_controller_t(vic2_t& sys) : _sys(sys) { }
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		io_controller_t _io_controller{ *this };

		vic2_t(const clock_source_t& clock);
      async::awaitable_func_t<> read_mem();
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_gpu();
   };

}
