
#pragma once

#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::mem::ram
{

	template <int NSize>
	class fixed_size_ram : public IMemoryInterface<fixed_size_ram<NSize>>
	{
	public:
		using Data = typename fixed_size_ram::IMemoryInterface::Data;
		using Address = typename fixed_size_ram::IMemoryInterface::Address;

		Data read(Address address) override
		{
			address &= Address(NSize -
									 1); // mwToDo: specify number of address line bits and use that instead.
			return _data[asUnderlyingType(address)];
		}

		void write(Address address, Data data) override
		{
			address &= Address(NSize -
									 1); // mwToDo: specify number of address line bits and use that instead.
			_data[asUnderlyingType(address)] = data;
		}

	private:
		std::array<types::Byte, NSize> _data;
	};

}
