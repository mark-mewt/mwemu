
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/diag/log.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"

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

	//gfx::image_t::size_t vic2_656x_t::display_size() const {
	//	return gfx::image_t::size_t{
	//		._width = gfx::image_t::width_t(_config.visible_scanline_width()),
	//		._height = gfx::image_t::height_t(_config.visible_scanline_count())
	//	};
	//}

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

   /* vic2_656x_t::vic2_656x_t(const clock_source_t& clock) //, vic2_model_t model)
		 : _clock(clock)//, _model(model)//, _config(vic2_config_t::get(model))
   {
      logger().log("vic2_656x_t::construct");
   }*/
	/*async::future<> vic2_656x_t::read_mem()
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
			if (raster_y == 0)
				break;
		}
   }*/


	/*
	
0	Black	#000000
1	White	#FFFFFF
2	Red	#9F4E44
3	Cyan	#6ABFC6
4	Purple	#A057A3
5	Green	#5CAB5E
6	Blue	#50459B
7	Yellow	#C9D487
8	Orange	#a1683c
9	Brown	#6D5412
10	Light Red	#CB7E75
11	Dark-Gray	#626262
12	Mid-Gray	#898989
13	Light Green	#9AE29B
14	Light Blue	#887ECB
15	Light-Gray	#ADADAD

	*/

	static types::colour_t vic2_colours[] = {
		{ 0x00, 0x00, 0x00 },
		{ 0xff, 0xff, 0xff },
		{ 0x9f, 0x4e, 0x44 },
		{ 0x6a, 0xbf, 0xc6 },
		{ 0xa0, 0x57, 0xa3 },
		{ 0x5c, 0xab, 0x5e },
		{ 0x50, 0x45, 0x9b },
		{ 0xc9, 0xd4, 0x87 },
		{ 0xa1, 0x68, 0x3c },
		{ 0x6d, 0x54, 0x12 },
		{ 0xcb, 0x7e, 0x75 },
		{ 0x62, 0x62, 0x62 },
		{ 0x89, 0x89, 0x89 },
		{ 0x9a, 0xe2, 0x9b },
		{ 0x88, 0x7e, 0xcb },
		{ 0xad, 0xad, 0xad },
	};

	void vic2_656x_t::generate_frame(host_t::frame_t& frame) {
		// tick some clock cycles...
		for (auto& row : frame._pixels.rows()) {
			for (auto& pixel : row) {
				pixel = vic2_colours[14];
			}
		}
	}

	async::future<> vic2_656x_t::run_gpu(host_t& host)
   {

		auto& config = co_await host.events.initialising;
		auto& vic_config = get_config();
		config.display_size = {
			._width = gfx::image_t::width_t(vic_config.visible_scanline_width()),
			._height = gfx::image_t::height_t(vic_config.visible_scanline_count())
		};

		for (;;) {
			auto& frame = co_await host.events.need_frame;
			generate_frame(frame);
		}
   }

}
