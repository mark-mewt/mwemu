
#pragma once

#include "mewt/types/concepts.h"
#include "mewt/types/int_types.h"

namespace mewt::emu::mem
{

	template <int NAddressBits, int NDataBits>
	struct BusSpec
	{
		const int kAddressBits = NAddressBits;
		const int kDataBits = NDataBits;
		using Address = types::UInt<NAddressBits>;
		using Data = types::UInt<NDataBits>;
	};

	template <int NAddressBits, int NDataBits>
	constexpr auto isBusSpec(const BusSpec<NAddressBits, NDataBits>& /*unused*/) { return true; }

	template <typename TType>
	concept is_BusSpec = requires(TType bus_spec) { isBusSpec(bus_spec); };

}
