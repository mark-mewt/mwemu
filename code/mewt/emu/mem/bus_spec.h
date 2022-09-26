
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

	template <typename TType>
	concept is_bus_spec = requires(TType x) {
									 requires std::is_same_v<decltype(BusSpec(x)), TType>;
								 };

}
