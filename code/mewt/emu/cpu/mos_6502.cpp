
#include "emu/cpu/mos_6502.h"
#include "types/intrusive_stack.h"
#include "async/awaitable_func.h"
#include "async/func_awaiter.h"
#include "emu/clock.h"
#include "diag/log.h"

#ifdef __clang__
#define __cpp_lib_coroutine 1
#endif

namespace std::experimental
{
   using namespace std;
}

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

   mos6502_t::mos6502_t(const clock_source_t& clock)
      : _clock(clock)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
   }

   async::awaitable_func_t<uint8_t> mos6502_t::read_mem()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
      co_return 42;
   }

   async::awaitable_func_t<> mos6502_t::write_mem(uint8_t v)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
   }

   async::awaitable_func_t<> mos6502_t::run_inst()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      auto v = co_await read_mem();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 4);
      co_await write_mem(v);
      logger().log("%s: %d", __FUNCTION__, 5);
   }

   async::awaitable_func_t<> mos6502_t::run_cpu()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 3);
   }

}
