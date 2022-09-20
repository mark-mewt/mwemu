
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"

namespace mewt::emu::chip::mos_65xx
{

   class sid_6581_t
   {
   private:
      const clock_source_t& _clock;
   public:

		struct io_controller_t : public MemoryInterface
		{
			sid_6581_t& _sys;
			struct regs_t
			{
				// https://www.c64-wiki.com/wiki/SID
				struct voice_t
				{
					Data _frequency_low;
					Data _frequency_high;
					Data _pulse_wave_duty_cycle_low;
					Data _pulse_wave_duty_cycle_high;
					Data _control_register;
					Data _attack_decay;
					Data _sustain_release;
				};
				voice_t _voice[3];
				Data _filter_cutoff_freq_low;
				Data _filter_cutoff_freq_high;
				Data _filter_resonance_routing;
				Data _filter_mode_volume;
				Data _paddle_x;
				Data _paddle_y;
				Data _oscillator_voice_3;
				Data _envelope_voice_3;
			};
			static_assert(sizeof(regs_t) == 0x1d);
			regs_t _regs{ };
			io_controller_t(sid_6581_t& sys) : _sys(sys) { }
			Data read(Address address) override final;
			void write(Address address, Data data) override final;
		};

		io_controller_t _io_controller{ *this };

		sid_6581_t(const clock_source_t& clock);
      async::future<> read_mem();
      async::future<> run_inst();
      async::future<> run_gpu();
   };

}
