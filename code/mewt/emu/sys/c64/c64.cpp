
#include "mewt/emu/sys/c64/c64.h"

#include "mewt/async/future_promise.h"
#include "mewt/diag/log.h"

namespace mewt::emu::sys::c64
{

	// https://www.pagetable.com/c64ref/c64disasm/

	/*void c64_t::run_sys() {

		_basic_rom.load_rom("emu/sys/c64/64c.251913-01.bin");
		_kernel_rom.load_rom("emu/sys/c64/64c.251913-01.bin", 8 * 1024);


		logger().log("%s: %d", __FUNCTION__, 0);
		auto cr = _cpu.run_cpu();
		auto gr = _vic2.run_gpu();
		logger().log("%s: %d", __FUNCTION__, 1);
		_clock.run();
		logger().log("%s: %d", __FUNCTION__, 2);
	}*/

	c64_t::c64_t(chip::mos_65xx::vic2_656x_t& vic2)
		 : _vic2(vic2)
	{
	}

	void c64_t::initSys(IHost& host)
	{

		_basic_rom.load_rom("emu/sys/c64/64c.251913-01.bin");
		_kernel_rom.load_rom("emu/sys/c64/64c.251913-01.bin", 8 * 1024);


		// logger().log("%s: %d", __FUNCTION__, 0);
		auto cr = _cpu.run_cpu();
		auto gr = _vic2.runGpu(host);
		// logger().log("%s: %d", __FUNCTION__, 1);
		//_clock.run();
		// logger().log("%s: %d", __FUNCTION__, 2);
	}

	// gfx::image_t::size_t c64_t::display_size() const {
	//	return _vic2.display_size();
	// }

	MemoryInterface& c64_t::memory_device(memory_device_t device_type)
	{
		switch (device_type)
		{
		case memory_device_t::Ram:
			return _ram;
		case memory_device_t::KernalRom:
			return _kernel_rom;
		case memory_device_t::BasicRom:
			return _basic_rom;
		case memory_device_t::IO:
			return _io_controller;
		default:
			throw std::exception("implement");
		}
	}

	template <typename TType, std::size_t NCount>
	class Array : public std::array<TType, NCount>
	{
		using Super = std::array<TType, NCount>;

	public:
		using Super::Super;
		constexpr Array(Super&& rhs) : Super(std::move(rhs)) {}
		using Address = types::OpaqueInt<std::bit_width(NCount - 1)>;
		inline auto operator[](Address address) const
		{
			return Super::operator[](asUnderlyingType(address));
		}
		inline auto operator[](Address address)
			 -> auto&
		{
			return Super::operator[](asUnderlyingType(address));
		}
	};
	template <typename TType, std::size_t NCount>
	Array(std::array<TType, NCount>) -> Array<TType, NCount>;

	c64_t::cpu_memory_controller_t::memory_region_t c64_t::cpu_memory_controller_t::address_region(Address address)
	{
		using mr = memory_region_t;
		constexpr Array regions = std::to_array<mr>({ mr::LoRam, mr::MidRam, mr::MidRam, mr::MidRam,
																	 mr::MidRam, mr::MidRam, mr::MidRam, mr::MidRam,
																	 mr::LoCart, mr::LoCart, mr::Basic, mr::Basic,
																	 mr::HiRam, mr::IO, mr::Kernal, mr::Kernal });
		return regions[highBits<4>(address)];
	}

	Address c64_t::cpu_memory_controller_t::address_mask(memory_device_t device)
	{
		constexpr auto blocks = std::to_array<std::uint32_t>({ 0, 16, 13, 13, 13, 12, 13, 12 });
		return (Address)((1 << blocks[(std::uint32_t)device]) - 1);
	}

	c64_t::memory_device_t c64_t::cpu_memory_controller_t::mapped_device(memory_region_t region)
	{
		bool _EXROM = _sys._system_flags[system_flag_t::ExpansionRom];
		bool _GAME = _sys._system_flags[system_flag_t::ExpansionGame];
		bool LORAM = _sys._port_bits[port_reg_t::LoRam];
		bool HIRAM = _sys._port_bits[port_reg_t::HiRam];
		bool CHAREN = _sys._port_bits[port_reg_t::CharEn];
		switch (region)
		{
		case memory_region_t::LoRam:
			return memory_device_t::Ram;
		case memory_region_t::MidRam:
		case memory_region_t::HiRam:
			if (_EXROM && !_GAME)
				return memory_device_t::None;
			else
				return memory_device_t::Ram;
		case memory_region_t::LoCart:
			if (_EXROM)
			{
				if (_GAME)
					return memory_device_t::Ram;
				else
					return memory_device_t::CartRomLo;
			}
			else
			{
				if (LORAM && HIRAM)
					return memory_device_t::CartRomLo;
				else
					return memory_device_t::Ram;
			}
		case memory_region_t::Basic:
			if (_EXROM && !_GAME)
				return memory_device_t::None;
			else if (!_EXROM && !_GAME && HIRAM)
				return memory_device_t::CartRomHi;
			else if (LORAM && HIRAM)
				return memory_device_t::BasicRom;
			else
				return memory_device_t::Ram;
		case memory_region_t::IO:
			if (_EXROM && !_GAME)
				return memory_device_t::IO;
			else if (!LORAM && !HIRAM)
				return memory_device_t::Ram;
			else if (CHAREN)
				return memory_device_t::IO;
			else if (!_GAME)
				return memory_device_t::Ram;
			else
				return memory_device_t::CharRom;
		case memory_region_t::Kernal:
			if (_EXROM && !_GAME)
				return memory_device_t::CartRomHi;
			else if (LORAM)
				return memory_device_t::KernalRom;
			else
				return memory_device_t::Ram;
		default:
			throw std::exception("fail");
		}
	}

	Data c64_t::cpu_memory_controller_t::read(Address address)
	{
		if (address == Address::Zero)
			return Data(_sys._port_write_enable.rawBits());
		if (address == Address(1))
			return Data(_sys._port_bits.rawBits());
		auto region = address_region(address);
		auto device = mapped_device(region);
		address &= address_mask(device);
		return _sys.memory_device(device).read(address);
		/*memory_interface<bus_spec_t>* mem_interface = &_sys._ram;
		address_t address_offset = 0;
		switch (address >> 12)
		{
		case 0xa:
		case 0xb:
			if (_sys._port_bits[port_reg_t::LoRam])
			{
				mem_interface = &_sys._basic_rom;
				address_offset = 0xa000;
			}
			break;
		case 0xe:
		case 0xf:
			if (_sys._port_bits[port_reg_t::HiRam])
			{
				mem_interface = &_sys._kernel_rom;
				address_offset = 0xe000;
			}
			break;
		case 0xd:
			if (_sys._port_bits[port_reg_t::CharEn])
			{
				mem_interface = &_sys._character_rom;
				address_offset = 0xd000;
				throw std::exception("Implement IO ports");
			}
			break;
		default:
			break;
		}
		return mem_interface->read(address - address_offset);*/
	}

	void c64_t::cpu_memory_controller_t::write(Address address, Data data)
	{
		if (address == Address::Zero)
			_sys._port_write_enable.rawBits() = asUnderlyingType(data);
		else if (address == Address(1))
			_sys._port_bits.rawBits() = asUnderlyingType(data);
		auto region = address_region(address);
		auto device = mapped_device(region);
		address &= address_mask(device);
		_sys.memory_device(device).write(address, data);
		/*else if (((address & 0xf000) == 0xd000) && _sys._port_bits[port_reg_t::CharEn])
		{
			throw std::exception("Implement IO ports");
		}
		else
			_sys._ram.write(address, data);*/
	}

	Data c64_t::io_controller_t::read(Address address)
	{
		return device_at(address).read(address);
	}

	void c64_t::io_controller_t::write(Address address, Data data)
	{
		device_at(address).write(address, data);
	}

	MemoryInterface& c64_t::io_controller_t::device_at(Address address)
	{
		auto page = lowNibble(highByte(address));
		if (page < types::HalfByte(4))
			return _sys._vic2.ioController();
		else if (page < types::HalfByte(8))
			return _sys._sid._io_controller;
		else if (page < types::HalfByte(12))
			return _sys._color_ram;
		else if (page == types::HalfByte(12))
			return _sys._cia1_controller;
		else if (page == types::HalfByte(13))
			return _sys._cia2_controller;
		else if (page == types::HalfByte(14))
			return _sys._io1_controller;
		else
			return _sys._io2_controller;
	}

	Data c64_t::dummy_controller_t::read(Address address)
	{
		throw std::exception("implement");
	}

	void c64_t::dummy_controller_t::write(Address address, Data data)
	{
		throw std::exception("implement");
	}

}
