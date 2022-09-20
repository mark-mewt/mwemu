
#pragma once

#include "mewt/emu/mem/bus_spec.h"

namespace mewt::emu::mem
{

	template <is_bus_spec _BusSpec>
	struct IMemoryInterface
	{

		using Data = typename _BusSpec::Data;
		using Address = typename _BusSpec::Address;

		// mwToDo: is there a more efficient way to do this than with v-table indirection?
		virtual auto read(Address address) -> Data = 0;
		virtual void write(Address address, Data data) = 0;
	};

}
