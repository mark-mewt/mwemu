
#include "emu/sys/c64.h"
#include "diag/log.h"
#include "async/awaitable_func.h"

namespace mewt::emu::sys
{

   void c64_t::run_sys()
   {

      _basic_rom.load_rom("emu/rom/64c.251913-01.bin");
      _kernel_rom.load_rom("emu/rom/64c.251913-01.bin", 8 * 1024);


      logger().log("%s: %d", __FUNCTION__, 0);
      auto cr = _cpu.run_cpu();
      auto gr = _vic2.run_gpu();
      //run_async([&]() { return cpu.cpu_runner(); });
      //run_async([&]() { gpu.gpu_runner(); });
      logger().log("%s: %d", __FUNCTION__, 1);
      _clock.run();
      logger().log("%s: %d", __FUNCTION__, 2);
   }

   memory_interface<c64_t::bus_spec_t>& c64_t::memory_device(memory_device_t device_type)
   {
      switch (device_type)
      {
      case memory_device_t::Ram:
         return _ram;
      case memory_device_t::KernalRom:
         return _kernel_rom;
      case memory_device_t::IO:
         return _io_controller;
      default:
         throw std::exception("implement");
      }
   }

   c64_t::cpu_memory_controller_t::memory_region_t c64_t::cpu_memory_controller_t::address_region(address_t address)
   {
      using mr = memory_region_t;
      static mr regions[] = {
         mr::LoRam, mr::MidRam, mr::MidRam, mr::MidRam,
         mr::MidRam, mr::MidRam, mr::MidRam, mr::MidRam,
         mr::LoCart, mr::LoCart, mr::Basic, mr::Basic,
         mr::HiRam, mr::IO, mr::Kernal, mr::Kernal
      };
      return regions[address >> 12];
   }

   c64_t::address_t c64_t::cpu_memory_controller_t::address_mask(memory_device_t device)
   {
      static data_t blocks[] = { 0, 16, 13, 13, 13, 12, 13, 12 };
      return (address_t)((1 << blocks[(data_t)device]) - 1);
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
         else {
            if(LORAM && HIRAM)
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

   c64_t::data_t c64_t::cpu_memory_controller_t::read(address_t address)
   {
      if (address == 0)
         return _sys._port_write_enable;
      if (address == 1)
         return _sys._port_bits;
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

   void c64_t::cpu_memory_controller_t::write(address_t address, data_t data)
   {
      if (address == 0)
         _sys._port_write_enable = data;
      else if (address == 1)
         _sys._port_bits = data;
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

   c64_t::data_t c64_t::io_controller_t::read(address_t address)
   {
      return device_at(address).read(address);
   }

   void c64_t::io_controller_t::write(address_t address, data_t data)
   {
      device_at(address).write(address, data);
   }

   memory_interface<c64_t::bus_spec_t>& c64_t::io_controller_t::device_at(address_t address)
   {
      auto page = (address >> 8) & 0xf;
      if (page < 4)
         return _sys._vic2._io_controller;
      else if (page < 8)
         return _sys._sid._io_controller;
      else if (page < 12)
         return _sys._color_io_controller;
      else if (page == 12)
         return _sys._cia1_controller;
      else if (page == 13)
         return _sys._cia2_controller;
      else if (page == 14)
         return _sys._io1_controller;
      else
         return _sys._io2_controller;
   }

   c64_t::data_t c64_t::dummy_controller_t::read(address_t address)
   {
      throw std::exception("implement");
   }

   void c64_t::dummy_controller_t::write(address_t address, data_t data)
   {
      throw std::exception("implement");
   }

}

