
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"
#include "mewt/diag/log.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/chip/clock/clock.h"

namespace mewt::emu::chip::mos_65xx {

   Data sid_6581_t::io_controller_t::read(Address address)
   {
      // https://www.c64-wiki.com/wiki/SID
      address &= 0xff;
      return *((Data*)&_regs + address);
   }

   void sid_6581_t::io_controller_t::write(Address address, Data data)
   {
      address &= 0xff;
      if (address >= 0x19)
         return;
      *((Data*)&_regs + address) = data;
   }

   sid_6581_t::sid_6581_t(const clock_source_t& clock)
      : _clock(clock)
   {
      //logger().log("sid_6581_t::construct");
   }
   async::Future<> sid_6581_t::read_mem()
   {
      //logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.nextTick();
      //logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.nextTick();
      //logger().log("%s: %d", __FUNCTION__, 2);
   }
   async::Future<> sid_6581_t::run_inst()
   {
      //logger().log("%s: %d", __FUNCTION__, 0);
      co_await read_mem();
      //logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.nextTick();
      //logger().log("%s: %d", __FUNCTION__, 2);
      co_await _clock.nextTick();
      //logger().log("%s: %d", __FUNCTION__, 3);
   }
   async::Future<> sid_6581_t::run_gpu()
   {
      //logger().log("%s: %d", __FUNCTION__, 0);
      co_await run_inst();
      //logger().log("%s: %d", __FUNCTION__, 1);
      co_await run_inst();
      //logger().log("%s: %d", __FUNCTION__, 2);
   }

}
