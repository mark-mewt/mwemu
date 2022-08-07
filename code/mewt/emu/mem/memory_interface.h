
#pragma once

#include "mewt/emu/mem/bus_spec.h"

namespace mewt::emu::mem
{

	template<is_bus_spec _BusSpec>
	struct memory_interface
	{

		using data_t = typename _BusSpec::data_t;
		using address_t = typename _BusSpec::address_t;

		// #todo: is there a more efficient way to do this than with v-table indirection?
		virtual data_t read(address_t address) = 0;
		virtual void write(address_t address, data_t data) = 0;

	};

}
