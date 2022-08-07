
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/diag/log.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/chip/clock/clock.h"

namespace mewt::emu::chip::mos_65xx
{

	// http://www.zimmers.net/cbmpics/cbm/c64/vic-ii.txt

	/*
	*
	* mwToDo: both vic2 and 6510 make a memory access in every cycle
	* mwToDo: vic2 accesses memory in first half of cpu cycle, 6510 accesses it in second half
	* mwToDo: set ba low 3 cycles before vic2 needs to steal bus access from cpu. 6510 should check this when reading data only, and stop until it goes high again.
	* 
	*/

   data_t vic2_656x_t::io_controller_t::read(address_t address)
   {
      // https://www.c64-wiki.com/wiki/Page_208-211
      address &= 0x3f;
      if (address >= 0x30)
         return 0xff;
      return *((data_t*)&_chip._regs + address);
   }

   void vic2_656x_t::io_controller_t::write(address_t address, data_t data)
   {
      address &= 0x3f;
      if (address >= 0x30)
         return;
      *((data_t*)&_chip._regs + address) = data;
   }

   vic2_656x_t::vic2_656x_t(const clock_source_t& clock)
      : _clock(clock)
   {
      logger().log("vic2_656x_t::construct");
   }
	async::future<> vic2_656x_t::read_mem()
   {
      logger().log("%s: %d", __FUNCTION__, 0);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 1);
      co_await _clock.next_tick();
      logger().log("%s: %d", __FUNCTION__, 2);
   }
	async::future<> vic2_656x_t::run_scanline(uint16_t raster_y) {
		_regs._raster = raster_y & 0xff;
		_regs._control_reg_1._rst8 = (raster_y & 0x100);
		for (int i = 0; i < 64; ++i) {
			co_await _clock.next_tick();
		}
	}
	async::future<> vic2_656x_t::run_frame()
   {
		uint16_t raster_y = 0;
		for (;;) {
			co_await run_scanline(raster_y);
			raster_y = (raster_y + 1) & 0x1ff;
		}
   }
	async::future<> vic2_656x_t::run_gpu()
   {
		for (;;) {
			co_await run_frame();
		}
   }

}
