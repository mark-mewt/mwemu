
#pragma once

#include "mewt/emu/mem/bus_spec.h"
#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::chip::mos_65xx {

	using bus_spec_t = mem::bus_spec<16, 8>;
	using memory_interface_t = mem::memory_interface<bus_spec_t>;
	using address_t = bus_spec_t::address_t;
	using data_t = bus_spec_t::data_t;

}