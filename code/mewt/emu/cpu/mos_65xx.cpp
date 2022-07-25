
#include "emu/cpu/mos_65xx.h"
#include "types/intrusive_stack.h"
#include "async/awaitable_func.h"
#include "async/func_awaiter.h"
#include "emu/clock.h"
#include "diag/log.h"

/*
* 
* ToDo:
* 
* - Write a splitter for clock cycles, so we can run things on sub-cycles.
* - Can we separate the list of awaiters from the clock source (and remove the mutable)?
* - Get C64 rom and start emulating it
* - Start emulating the Vic-2
* - Enable all warnings in compiler - need to shift out the win32 stuff first
* - Make address sanitizer work
* 
*/

namespace mewt::emu::cpu
{

   mos65xx_t::mos65xx_t(const clock_source_t& clock, memory_interface<bus_spec_t>& memory_interface)
      : _clock(clock), _memory_interface(memory_interface)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
   }

   async::awaitable_func_t<mos65xx_t::data_t> mos65xx_t::read_data(address_t address)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      co_return _memory_interface.read(address);
   }

   async::awaitable_func_t<mos65xx_t::address_t> mos65xx_t::read_address(address_t address)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      auto low = co_await read_data(address);
      auto high = co_await read_data(address + 1);
      co_return (address_t)low | ((address_t)high << 8);
   }

   async::awaitable_func_t<> mos65xx_t::write_mem(uint8_t v)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
   }

   async::awaitable_func_t<> mos65xx_t::run_inst()
   {
      auto inst = co_await read_data(_pc);
      logger().log("0x%x: %x", _pc, inst);
      ++_pc;
      switch (inst)
      {
      case 0:
         break;
      }
   }

   async::awaitable_func_t<> mos65xx_t::run_cpu()
   {
      _pc = co_await read_address(0xfffc);
      logger().log("%s: %d", __FUNCTION__, 0);
      for (;;)
      {
         co_await run_inst();
      }
   }

}
