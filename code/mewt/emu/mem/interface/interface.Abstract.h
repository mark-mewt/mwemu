
#pragma once

#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::mem::interface
{

	template <BusSpec NBusSpec>
	struct Abstract
	{

		using AddressSpace = AddressSpace<NBusSpec>;
		using Address = typename AddressSpace::Address;

		virtual auto read(Address address) -> Byte = 0;

		virtual void write(Address address, Byte data) = 0;

	};

}
