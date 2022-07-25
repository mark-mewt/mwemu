
#pragma once

#include "emu/rom/fixed_size_rom.h"
#include "emu/ram/fixed_size_ram.h"
#include "emu/clock.h"
#include "emu/cpu/mos_6510.h"
#include "emu/gpu/vic2.h"

namespace mewt::emu::sys
{

	class c64_t
	{

	public:

		void run_sys();

	private:

		using bus_spec_t = bus_spec<16, 8>;
		using address_t = bus_spec_t::address_t;
		using data_t = bus_spec_t::data_t;

		struct cpu_memory_controller_t : public memory_interface<bus_spec_t>
		{
			c64_t& _sys;
			cpu_memory_controller_t(c64_t& sys) : _sys(sys) { }
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};
		cpu_memory_controller_t _cpu_memory_controller{ *this };
		clock_source_t _clock;
		cpu::mos6510_t _cpu{ _clock, _cpu_memory_controller };
		gpu::vic2_t _gpu{ _clock };
		rom::fixed_size_rom<8 * 1024, bus_spec_t> _basic_rom;
		rom::fixed_size_rom<8 * 1024, bus_spec_t> _kernel_rom;
		rom::fixed_size_rom<4 * 1024, bus_spec_t> _character_rom;
		ram::fixed_size_ram<64 * 1024, bus_spec_t> _ram;

		data_t _data_direction_reg = 0x2f;
		data_t _io_port_reg = 0x7;

	};

}
