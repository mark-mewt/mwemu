
#pragma once

#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/types/address_space.h"
#include "mewt/emu/mem/interface/interface.RegisterStruct.h"

namespace mewt::emu::chip::mos_65xx
{

	//static constexpr mem::BusSpec kCIABusSpec { .address_bits = { 4 } };

	//class CIA;
	//constexpr auto getBusSpecForDevice(types::traits::TypeId<CIA>) { return kCIABusSpec; }

	class CIA
	{
	public:
		//class MemoryController;

	private:
		struct Registers
		{
			Byte data_port_a { 0 };
			Byte data_port_b { 0 };
			Byte data_direction_a { 0 };
			Byte data_direction_b { 0 };
			Byte timer_a_low { 0 };
			Byte timer_a_high { 0 };
			Byte timer_b_low { 0 };
			Byte timer_b_high { 0 };
			Byte rtc_tenth_seconds { 0 };
			Byte rtc_seconds { 0 };
			Byte rtc_minutes { 0 };
			Byte rtc_hours { 0 };
			Byte serial_shift_register { 0 };
			Byte interrupt_control_status { 0 };
			Byte control_timer_a { 0 };
			Byte control_timer_b { 0 };
		};

		#if 0
		friend constexpr auto
		declareEquivalentSpaces(types::traits::TypeId<mem::AddressSpace<CIA>> /*unused*/,
										types::traits::TypeId<types::traits::Layout<Registers>> /*unused*/)
		{
			return true;
		}
		#endif

		//Registers _regs;

		//friend class RegisterMemoryController<CIA>;
		mem::interface::RegisterStruct<Registers> _memoryController;

	};

}
