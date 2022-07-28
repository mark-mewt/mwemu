
#pragma once

#include "mewt/async/awaitable_func.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"

namespace mewt::emu::chip::mos_65xx
{

   class sid_6581_t
   {
   private:
      const clock_source_t& _clock;
   public:

		struct io_controller_t : public memory_interface_t
		{
			sid_6581_t& _sys;
			struct regs_t
			{
				// https://www.c64-wiki.com/wiki/SID
				struct voice_t
				{
					data_t _frequency_low;
					data_t _frequency_high;
					data_t _pulse_wave_duty_cycle_low;
					data_t _pulse_wave_duty_cycle_high;
					data_t _control_register;
					data_t _attack_decay;
					data_t _sustain_release;
				};
				voice_t _voice[3];
				data_t _filter_cutoff_freq_low;
				data_t _filter_cutoff_freq_high;
				data_t _filter_resonance_routing;
				data_t _filter_mode_volume;
				data_t _paddle_x;
				data_t _paddle_y;
				data_t _oscillator_voice_3;
				data_t _envelope_voice_3;
			};
			static_assert(sizeof(regs_t) == 0x1d);
			regs_t _regs;
			io_controller_t(sid_6581_t& sys) : _sys(sys) { }
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		io_controller_t _io_controller{ *this };

		sid_6581_t(const clock_source_t& clock);
      async::awaitable_func_t<> read_mem();
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_gpu();
   };

}
