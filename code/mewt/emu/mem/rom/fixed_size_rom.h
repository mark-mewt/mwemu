
#pragma once

#include "mewt/io/reader.h"
#include "mewt/emu/mem/memory_interface.h"

#include <string>

namespace mewt::emu::mem::rom
{

	template <int _Size, is_BusSpec _BusSpec>
	class fixed_size_rom : public IMemoryInterface<_BusSpec>
	{

	public:
		using Data = typename IMemoryInterface<_BusSpec>::Data;
		using Address = typename IMemoryInterface<_BusSpec>::Address;

		inline void load_rom(const std::string_view& name, size_t offset = 0)
		{
			io::reader_t reader;
			reader.read_data(name, _data, _Size, offset);
		}

		Data read(Address address) override
		{
			return *(Data*)(_data + address);
		}

		void write(Address address, Data data) override
		{
		}

	private:
		char _data[_Size]{ 0 };
	};

}
