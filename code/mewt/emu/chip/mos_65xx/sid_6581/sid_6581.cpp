
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"
#include "mewt/diag/log.h"
#include "mewt/async/awaitable_func.h"
#include "mewt/async/func_awaiter.h"
#include "mewt/emu/chip/clock/clock.h"

namespace mewt::emu::chip::mos_65xx {

   data_t sid_6581_t::io_controller_t::read(address_t address)
   {
      // https://www.c64-wiki.com/wiki/SID
      address &= 0xff;
      return *((data_t*)&_regs + address);
   }

   void sid_6581_t::io_controller_t::write(address_t address, data_t data)
   {
      address &= 0xff;
      if (address >= 0x19)
         return;
      *((data_t*)&_regs + address) = data;
   }

   sid_6581_t::sid_6581_t(const clock_source_t& clock)
      : _clock(clock)
   {
      logger().log("sid_6581_t::construct");
   }
   async::awaitable_func_t<> sid_6581_t::read_mem()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
   }
   async::awaitable_func_t<> sid_6581_t::run_inst()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await read_mem();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 3);
   }
   async::awaitable_func_t<> sid_6581_t::run_gpu()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await run_inst();
      logger().log("%s: %d", __FUNCTION__, 2);
   }

}
