
#include "emu/gpu/vic2.h"
#include "diag/log.h"
#include "async/awaitable_func.h"
#include "async/func_awaiter.h"
#include "emu/clock.h"

namespace mewt::emu::gpu
{

   vic2_t::vic2_t(const clock_source_t& clock)
      : _clock(clock)
   {
      logger().log("vic2_t::construct");
   }
   async::awaitable_func_t<> vic2_t::read_mem()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
   }
   async::awaitable_func_t<> vic2_t::run_inst()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await read_mem();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
   }
   async::awaitable_func_t<> vic2_t::run_gpu()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 2);
   }

}
