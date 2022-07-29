
#pragma once

#include "mewt/emu/mem/memory_interface.h"

namespace mewt::emu::mem::ram
{

	template<int _Size, is_bus_spec _BusSpec>
	class fixed_size_ram : public memory_interface<_BusSpec>
	{

	public:

		using data_t = memory_interface<_BusSpec>::data_t;
		using address_t = memory_interface<_BusSpec>::address_t;

		data_t read(address_t address) override
		{
			address &= (_Size - 1); // todo: specify number of address line bits and use that instead.
			return *(data_t*)(_data + address);
		}

		void write(address_t address, data_t data) override
		{
			address &= (_Size - 1); // todo: specify number of address line bits and use that instead.
			*(data_t*)(_data + address) = data;
		}

	private:

		char _data[_Size];

	};

}
