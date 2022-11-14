
#pragma once

#include "mewt/emu/mem/memory_interface.h"
#include "mewt/types/byte_span.h"

namespace mewt::emu::mem::interface
{

	namespace detail
	{

		template <typename TType>
		constexpr auto kBitsForType =
			 native::bitsRequiredForValue(traits::kSizeOf<TType>);

		template <typename TType>
		constexpr auto kAddressBitsForType = toSpace<mem::AddressBus>(kBitsForType<TType>);

		template<typename TType>
		constexpr auto kBusSpecForStruct =
			 mem::BusSpec<TType> { .address_bits = kAddressBitsForType<TType> };

	}

	template <typename TRegisters>
	class RegisterStruct
	{
		using AddressSpace = AddressSpace<detail::kBusSpecForStruct<TRegisters>>;

	public:
		using Address = typename AddressSpace::Address;

		inline auto read(Address address) -> Byte { return span()[address]; }

		inline void write(Address address, Byte data) { span()[address] = data; }

	private:
		TRegisters _regs;

		inline auto span() { return ByteSpan(_regs); }
	};

}
