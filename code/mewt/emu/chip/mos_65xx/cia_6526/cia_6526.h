
#pragma once

#include "mewt/emu/chip/mos_65xx/mos_65xx.h"

namespace mewt::emu::chip::mos_65xx
{

	struct cia_6526_t : public MemoryInterface
	{
		struct regs_t
		{
			Data _data_port_a;
			Data _data_port_b;
			Data _data_direction_a;
			Data _data_direction_b;
			Data _timer_a_low;
			Data _timer_a_high;
			Data _timer_b_low;
			Data _timer_b_high;
			Data _rtc_tenth_seconds;
			Data _rtc_seconds;
			Data _rtc_minutes;
			Data _rtc_hours;
			Data _serial_shift_register;
			Data _interrupt_control_status;
			Data _control_timer_a;
			Data _control_timer_b;
		};
		regs_t _regs{ 0 };
		Data read(Address address) override final;
		void write(Address address, Data data) override final;
	};

}
