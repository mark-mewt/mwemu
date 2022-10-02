
#pragma once

#include "mewt/emu/mem/bus_spec.h"
#include "mewt/types/interface.h"

namespace mewt::emu::mem
{

	template <is_BusSpec TBusSpec>
	struct IMemoryInterface : public types::IInterface
	{

		using Data = typename TBusSpec::Data;
		using Address = typename TBusSpec::Address;

		// mwToDo: is there a more efficient way to do this than with v-table indirection?
		virtual auto read(Address address) -> Data = 0;
		virtual void write(Address address, Data data) = 0;
	};

}
