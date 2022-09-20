
#pragma once

#include "mewt/emu/mem/bus_spec.h"
#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::chip::mos_65xx {

	using BusSpec = mem::BusSpec<16, 8>;
	using MemoryInterface = mem::IMemoryInterface<BusSpec>;
	using Address = BusSpec::Address;
	using Data = BusSpec::Data;
}