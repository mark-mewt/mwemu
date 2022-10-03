
#pragma once

#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::mem::ram
{

	template <int _Size, is_BusSpec _BusSpec>
	class fixed_size_ram : public IMemoryInterface<_BusSpec>
	{

	public:
		using Data = typename IMemoryInterface<_BusSpec>::Data;
		using Address = typename IMemoryInterface<_BusSpec>::Address;

		Data read(Address address) override
		{
			address &= Address(_Size - 1); // mwToDo: specify number of address line bits and use that instead.
			return _data[asUnderlyingType(address)];
		}

		void write(Address address, Data data) override
		{
			address &= Address(_Size - 1); // mwToDo: specify number of address line bits and use that instead.
			_data[asUnderlyingType(address)] = data;
		}

	private:
		std::array<types::Byte, _Size> _data;
	};

}
