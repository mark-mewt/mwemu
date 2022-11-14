
#pragma once

#include "mewt/emu/chip/mos_65xx/cia_6526/cia_6526.h"
#include "mewt/emu/chip/mos_65xx/cpu_6510/cpu_6510.h"
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/emu/host/host.h"
#include "mewt/emu/mem/ram/fixed_size_ram.h"
#include "mewt/emu/mem/rom/fixed_size_rom.h"
#include "mewt/emu/sys/c64/c64.decl.h"
#include "mewt/emu/sys/system.h"
#include "mewt/gfx/image.h"
#include "mewt/types/flags.h"

namespace mewt::emu::sys::c64
{

	using namespace chip::mos_65xx;

	// struct c64_config_t {
	//	vic2_model_t _vic2_model;
	// };

	class c64_t : public ISystem
	{
	public:
		c64_t(chip::mos_65xx::vic2_656x_t& vic2);

		// void run_sys();
		void initSys(IHost& host);

		// gfx::image_t::size_t display_size() const;

		virtual ~c64_t() = default;

	protected:
		// virtual chip::mos_65xx::vic2_656x_t& get_vic2() = 0;

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

		MemoryInterface& memory_device(memory_device_t device_type);

		struct cpu_memory_controller_t
			 : public mem::IMemoryInterface<kCPUBusSpec, cpu_memory_controller_t>
		{
			c64_t& _sys;
			enum class memory_region_t
			{
				LoRam,  // 0x0000 - 0x0FFF
				MidRam, // 0x1000 - 0x7FFF
				LoCart, // 0x8000 - 0x9FFF
				Basic,  // 0xA000 - 0xBFFF
				HiRam,  // 0xC000 - 0xCFFF
				IO,	  // 0xD000 - 0xDFFF
				Kernal, // 0xE000 - 0xFFFF
			};

			cpu_memory_controller_t(c64_t& sys)
				 : _sys(sys)
			{
			}

			static memory_region_t address_region(Address address);
			static Address address_mask(memory_device_t device);
			memory_device_t mapped_device(memory_region_t region);
			Data read(Address address) override final;
			void write(Address address, Data data) override final;
		};

		static constexpr mem::BusSpec kIOBusSpec { .address_bits = 12 };

		struct io_controller_t : public mem::IMemoryInterface<kIOBusSpec, io_controller_t>
		{
			c64_t& _sys;

			io_controller_t(c64_t& sys)
				 : _sys(sys)
			{
			}

			IMemoryInterface<BusSpec>& device_at(Address address);
			Data read(Address address) override final;
			void write(Address address, Data data) override final;
		};

		struct dummy_controller_t : public MemoryInterface
		{
			Data read(Address address) override final;
			void write(Address address, Data data) override final;
		};

		// c64_config_t _config;
		cpu_memory_controller_t _cpu_memory_controller { *this };
		chip::mos_65xx::vic2_656x_t& _vic2;
		// chip::clock_source_t _clock;
		chip::mos_65xx::cpu_6510_t _cpu { _vic2.cpuClock(), _cpu_memory_controller };
		// chip::mos_65xx::vic2_656x_t _vic2{ _clock, _config._vic2_model };
		mem::rom::fixed_size_rom<8 * 1024, BusSpec> _basic_rom;
		mem::rom::fixed_size_rom<8 * 1024, BusSpec> _kernel_rom;
		mem::rom::fixed_size_rom<4 * 1024, BusSpec> _character_rom;
		mem::ram::fixed_size_ram<64 * 1024, BusSpec> _ram;
		mem::ram::fixed_size_ram<1 * 1024, BusSpec> _color_ram;
		io_controller_t _io_controller { *this };

		chip::mos_65xx::sid_6581_t _sid { _vic2.cpuClock() };

		chip::mos_65xx::CIA _cia1_controller;
		chip::mos_65xx::CIA _cia2_controller;
		dummy_controller_t _io1_controller;
		dummy_controller_t _io2_controller;

		enum class port_reg_t : std::uint8_t
		{
			LoRam,
			HiRam,
			CharEn,
			CassetteWrite,
			CassetteSense, // 0 = PLAY button depressed
			CassetteMotor, // 0 = motor spind
		};

		types::flags<port_reg_t> _port_write_enable { 0x2f };
		types::flags<port_reg_t> _port_bits { 0x7 };

		enum class system_flag_t : std::uint8_t
		{
			ExpansionRom,	// Expansion Port _EXROM
			ExpansionGame, // Expansion Port _GAME
		};

		types::flags<system_flag_t> _system_flags { 0x3 };

		// data_t _data_direction_reg = 0x2f;
		// data_t _io_port_reg = 0x7;
	};

}
