
#pragma once

#include "mewt/io/reader.h"
#include "mewt/emu/mem/memory_interface.h"

#include <string>

namespace mewt::emu::mem::rom
{

	template<int _Size, is_bus_spec _BusSpec>
	class fixed_size_rom : public memory_interface<_BusSpec>
	{

	public:

		using data_t = memory_interface<_BusSpec>::data_t;
		using address_t = memory_interface<_BusSpec>::address_t;

		inline void load_rom(const std::string_view& name, size_t offset = 0)
		{
			io::reader_t reader;
			reader.read_data(name, _data, _Size, offset);
		}

		data_t read(address_t address) override
		{
			return *(data_t*)(_data + address);
		}

		void write(address_t address, data_t data) override
		{
		}

	private:

		char _data[_Size];

	};

}
