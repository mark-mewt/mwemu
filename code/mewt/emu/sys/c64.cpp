
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
      auto gr = _gpu.run_gpu();
      //run_async([&]() { return cpu.cpu_runner(); });
      //run_async([&]() { gpu.gpu_runner(); });
      logger().log("%s: %d", __FUNCTION__, 1);
      _clock.run();
      logger().log("%s: %d", __FUNCTION__, 2);
   }

   c64_t::data_t c64_t::cpu_memory_controller_t::read(address_t address)
   {
      if (address == 0)
         return _sys._data_direction_reg;
      if (address == 1)
         return _sys._io_port_reg;
      memory_interface<bus_spec_t>* mem_interface = &_sys._ram;
      address_t address_offset = 0;
      switch (address >> 12)
      {
      case 0xa:
      case 0xb:
         if (_sys._io_port_reg & 1)
         {
            mem_interface = &_sys._basic_rom;
            address_offset = 0xa000;
         }
         break;
      case 0xe:
      case 0xf:
         if (_sys._io_port_reg & 2)
         {
            mem_interface = &_sys._kernel_rom;
            address_offset = 0xe000;
         }
         break;
      case 0xd:
         if (_sys._io_port_reg & 4)
         {
            mem_interface = &_sys._character_rom;
            address_offset = 0xd000;
         }
         break;
      default:
         break;
      }
      return mem_interface->read(address - address_offset);
   }

   void c64_t::cpu_memory_controller_t::write(address_t address, data_t data)
   {
      if (address == 0)
         _sys._data_direction_reg = data;
      else if (address == 1)
         _sys._io_port_reg = data;
      else
         _sys._ram.write(address, data);
   }

}

