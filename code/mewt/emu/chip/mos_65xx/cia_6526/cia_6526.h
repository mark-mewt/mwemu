
#pragma once

#include "mewt/emu/chip/mos_65xx/mos_65xx.h"

namespace mewt::emu::chip::mos_65xx
{

	struct cia_6526_t : public memory_interface_t
	{
		struct regs_t
		{
			data_t _data_port_a;
			data_t _data_port_b;
			data_t _data_direction_a;
			data_t _data_direction_b;
			data_t _timer_a_low;
			data_t _timer_a_high;
			data_t _timer_b_low;
			data_t _timer_b_high;
			data_t _rtc_tenth_seconds;
			data_t _rtc_seconds;
			data_t _rtc_minutes;
			data_t _rtc_hours;
			data_t _serial_shift_register;
			data_t _interrupt_control_status;
			data_t _control_timer_a;
			data_t _control_timer_b;
		};
		regs_t _regs;
		data_t read(address_t address) override final;
		void write(address_t address, data_t data) override final;
	};

}
