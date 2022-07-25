
#pragma once

#include "types/int_types.h"
#include "types/concepts.h"

namespace mewt::emu
{

	template<int _AddressBits, int _DataBits>
	struct bus_spec
	{
		inline static int address_bits = _AddressBits;
		inline static int data_bits = _DataBits;
		using address_t = types::uint<_AddressBits>;
		using data_t = types::uint<_DataBits>;
	};

	namespace detail
	{
		template<int _AddressBits, int _DataBits>
		constexpr bool is_bus_spec_func(const bus_spec<_AddressBits, _DataBits>&) { return true; }
	}

	template<typename _Type>
	concept is_bus_spec = requires(_Type x) { detail::is_bus_spec_func(x); };

}
