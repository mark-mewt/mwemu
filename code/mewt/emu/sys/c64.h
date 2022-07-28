
#pragma once

#include "emu/rom/fixed_size_rom.h"
#include "emu/ram/fixed_size_ram.h"
#include "emu/clock.h"
#include "emu/cpu/mos_6510.h"
#include "emu/gpu/vic2.h"
#include "types/flags.h"
#include "emu/ic/mos_cia_6526.h"
#include "emu/mos/sid_6581.h"

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

		enum class memory_device_t
		{
			None,
			Ram,
			CartRomLo,
			CartRomHi,
			BasicRom,
			CharRom,
			KernalRom,
			IO
		};

		memory_interface<bus_spec_t>& memory_device(memory_device_t device_type);

		struct cpu_memory_controller_t : public memory_interface<bus_spec_t>
		{
			c64_t& _sys;
			enum class memory_region_t
			{
				LoRam,	// 0x0000 - 0x0FFF
				MidRam,	// 0x1000 - 0x7FFF
				LoCart,	// 0x8000 - 0x9FFF
				Basic,	// 0xA000 - 0xBFFF
				HiRam,	// 0xC000 - 0xCFFF
				IO,		// 0xD000 - 0xDFFF
				Kernal,	// 0xE000 - 0xFFFF
			};
			cpu_memory_controller_t(c64_t& sys) : _sys(sys) { }
			static memory_region_t address_region(address_t address);
			static address_t address_mask(memory_device_t device);
			memory_device_t mapped_device(memory_region_t region);
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		struct io_controller_t : public memory_interface<bus_spec_t>
		{
			c64_t& _sys;
			io_controller_t(c64_t& sys) : _sys(sys) { }
			memory_interface<bus_spec_t>& device_at(address_t address);
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		struct dummy_controller_t : public memory_interface<bus_spec_t>
		{
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		cpu_memory_controller_t _cpu_memory_controller{ *this };
		clock_source_t _clock;
		cpu::mos6510_t _cpu{ _clock, _cpu_memory_controller };
		gpu::vic2_t _vic2{ _clock };
		rom::fixed_size_rom<8 * 1024, bus_spec_t> _basic_rom;
		rom::fixed_size_rom<8 * 1024, bus_spec_t> _kernel_rom;
		rom::fixed_size_rom<4 * 1024, bus_spec_t> _character_rom;
		ram::fixed_size_ram<64 * 1024, bus_spec_t> _ram;
		io_controller_t _io_controller{ *this };

		mos::sid_6581_t _sid{ _clock };

		dummy_controller_t _color_io_controller;
		ic::mos_cia_6526_t _cia1_controller;
		ic::mos_cia_6526_t _cia2_controller;
		dummy_controller_t _io1_controller;
		dummy_controller_t _io2_controller;

		enum class port_reg_t
		{
			LoRam,
			HiRam,
			CharEn,
			CassetteWrite,
			CassetteSense,	// 0 = PLAY button depressed
			CassetteMotor, // 0 = motor spind
		};

		types::flags<port_reg_t, 8> _port_write_enable{ 0x2f };
		types::flags<port_reg_t, 8> _port_bits{ 0x7 };

		enum class system_flag_t
		{
			ExpansionRom,	// Expansion Port _EXROM
			ExpansionGame,	// Expansion Port _GAME
		};

		types::flags<system_flag_t, 8> _system_flags{ 0x3 };

		//data_t _data_direction_reg = 0x2f;
		//data_t _io_port_reg = 0x7;

	};

}
