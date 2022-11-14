
#pragma once

#include "mewt/emu/mem/memory_interface.h"
#include "mewt/io/reader.h"

#include <array>
#include <string>

namespace mewt::emu::mem::rom
{

	template <BusSpec NBusSpec, typename TName>
	class fixed_size_rom : public mem::IMemoryInterface<fixed_size_rom<NBusSpec, TName>>
	{
	public:
		// using Data = typename IMemoryInterface<_BusSpec>::Data;
		using Address = typename fixed_size_rom::IMemoryInterface::Address;

		inline void load_rom(const std::string_view& name, size_t offset = 0)
		{
			io::reader_t reader;
			reader.read_data(name, _data, offset);
		}

		types::Byte read(Address address) override
		{
			return _data[nativeValue(address)];
		}

		void write(Address address, Data data) override {}

	private:
		std::array<Data, _Size> _data;
	};

}
