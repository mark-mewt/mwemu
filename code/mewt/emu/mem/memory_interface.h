
#pragma once

#include "mewt/emu/mem/bus_spec.h"
#include "mewt/types/interface.h"
#include "mewt/types/native/native.Integer.h"
#include "mewt/types/numeric/numeric.Index.h"
#include "mewt/types/numeric/numeric.Integer.h"
#include "mewt/types/traits/traits.TypeId.h"
#include "mewt/emu/device/device.Info.h"

namespace mewt::emu::mem
{

	// template <typename TDevice>
	// constexpr auto getBusSpecForDevice(types::ClassId<TDevice>);

	template <typename TDevice>
	constexpr BusSpec kBusSpecFor = getBusSpecForDevice(types::traits::kTypeId<TDevice>);

/* template <typename TDeviceInfo>
	concept isMemoryDeviceInfo = requires(TDeviceInfo device_info) {
											  requires device::isDeviceInfo<TDeviceInfo>;
											  {
												  TDeviceInfo::kBusSpec
												  } -> std::same_as<BusSpec>;
										  };
										  */

	template <BusSpec NBusSpec>
	struct AddressSpace
	{
		static constexpr auto kBusSpec = NBusSpec; //		kBusSpecFor<TDevice>;
		using NativeAddressType = types::numeric::Integer<kBusSpec.address_bits, false>;

		friend constexpr auto
		getNumericSpaceGranularity(types::traits::TypeId<AddressSpace> /*unused*/)
		{
			return types::numeric::space::Granularity::Discrete;
		}

		friend constexpr auto getNativeTypeForUnit(types::traits::TypeId<types::native::Byte>,
														types::traits::TypeId<AddressSpace>)
			 -> types::traits::TypeId<NativeAddressType>;

		using Address = types::numeric::Index<types::native::Byte, AddressSpace>;
	};

	// template <typename TDevice>
	//  ///constexpr auto
	// getNumericSpaceGranularity(types::traits::TypeId<AddressSpace<TDevice>> /*unused*/)
	//{
	//	return types::numeric::space::Granularity::Discrete;
	// }

	// template <typename TDevice>
	// constexpr auto getNativeTypeForUnit(types::traits::TypeId<types::native::Byte>,
	//												types::traits::TypeId<AddressSpace<TDevice>>)
	//	 -> types::traits::TypeId<typename AddressSpace<TDevice>::NativeAddressType>;

	//template <BusSpec NBusSpec>
	//struct IMemoryInterface : public types::IInterface
	//{
		// using Data = NBusSpec.;
		// using Address = TBusSpec::Address;
		// enum class Address : types::FastestInt<kBusSpec.address_bits>;
		//using Address = BusAddress<TDevice>;
		// enum class Data : types::FastestInt<kBusSpec.data_bits>;
		// using Data = types::native::Byte;
		//using AddressSpace = AddressSpace<NBusSpec>;
		// mwToDo: is there a more efficient way to do this than with v-table indirection?
		// virtual auto read(Address address) -> Data = 0;
		// virtual void write(Address address, Data data) = 0;
	//};

}
