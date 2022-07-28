
#pragma once

#include "mewt/emu/mem/rom/fixed_size_rom.h"
#include "mewt/emu/mem/ram/fixed_size_ram.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/cpu_6510/cpu_6510.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/types/flags.h"
#include "mewt/emu/chip/mos_65xx/cia_6526/cia_6526.h"
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"

namespace mewt::emu::sys::c64
{

	using namespace chip::mos_65xx;

	class c64_t
	{

	public:

		void run_sys();

	private:

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

		memory_interface_t& memory_device(memory_device_t device_type);

		struct cpu_memory_controller_t : public memory_interface_t
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

		struct io_controller_t : public memory_interface_t
		{
			c64_t& _sys;
			io_controller_t(c64_t& sys) : _sys(sys) { }
			memory_interface<bus_spec_t>& device_at(address_t address);
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		struct dummy_controller_t : public memory_interface_t
		{
			data_t read(address_t address) override final;
			void write(address_t address, data_t data) override final;
		};

		cpu_memory_controller_t _cpu_memory_controller{ *this };
		chip::clock_source_t _clock;
		chip::mos_65xx::cpu_6510_t _cpu{ _clock, _cpu_memory_controller };
		chip::mos_65xx::vic2_656x_t _vic2{ _clock };
		mem::rom::fixed_size_rom<8 * 1024, bus_spec_t> _basic_rom;
		mem::rom::fixed_size_rom<8 * 1024, bus_spec_t> _kernel_rom;
		mem::rom::fixed_size_rom<4 * 1024, bus_spec_t> _character_rom;
		mem::ram::fixed_size_ram<64 * 1024, bus_spec_t> _ram;
		io_controller_t _io_controller{ *this };

		chip::mos_65xx::sid_6581_t _sid{ _clock };

		dummy_controller_t _color_io_controller;
		chip::mos_65xx::cia_6526_t _cia1_controller;
		chip::mos_65xx::cia_6526_t _cia2_controller;
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
