
#pragma once

#include "mewt/types/concepts.h"
#include "mewt/types/int_types.h"
#include "mewt/types/numeric/numeric.space.h"
#include "mewt/types/numeric/numeric.Count.h"
#include "mewt/types/native/native.Bit.h"

namespace mewt::emu::mem
{

	/*template <int NAddressBits, int NDataBits>
	struct BusSpec
	{
		const int kAddressBits = NAddressBits;
		const int kDataBits = NDataBits;
		using Address = types::OpaqueInt<NAddressBits>;
		using Data = types::OpaqueInt<NDataBits>;
	};*/

	// template <int NAddressBits, int NDataBits>
	// constexpr auto isBusSpec(const BusSpec<NAddressBits, NDataBits>& /*unused*/) { return true; }

	// template <typename TType>
	// concept is_BusSpec = requires(TType bus_spec) { isBusSpec(bus_spec); };

	struct AddressBus : public types::numeric::space::Discrete
	{
		friend constexpr auto getNativeTypeForUnit(types::traits::TypeId<types::native::Bit>, types::traits::TypeId<AddressBus>)
		 -> types::traits::TypeId<std::uint8_t>;
	};

	template<typename TDevice>
	struct BusSpec
	{
		const types::numeric::Count<types::native::Bit, AddressBus> address_bits;
		// const int data_bits;
	};

	template<typename TDevice>
	class DeviceInfo;

}
