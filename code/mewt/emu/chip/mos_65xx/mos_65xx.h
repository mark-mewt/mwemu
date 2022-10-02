
#pragma once

#include "mewt/emu/mem/bus_spec.h"
#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::chip::mos_65xx
{

	constexpr int kAddressBits = 16;
	constexpr int kDataBits = 8;
	using BusSpec = mem::BusSpec<kAddressBits, kDataBits>;
	using MemoryInterface = mem::IMemoryInterface<BusSpec>;
	using Address = BusSpec::Address;
	using Data = BusSpec::Data;
}