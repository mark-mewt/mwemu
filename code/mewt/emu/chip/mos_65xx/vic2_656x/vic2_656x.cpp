
#include "mewt/async/future_promise.h"
#include "mewt/diag/log.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"
#include "mewt/emu/host/host.impl.h"
#include "mewt/async/event.impl.h"

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

	auto vic2_656x_t::IoControllerT::read(Address address) -> Data
   {
      // https://www.c64-wiki.com/wiki/Page_208-211
      address &= 0x3f;
      if (address >= 0x30)
         return 0xff;
      return *((Data*)&_chip._regs + address);
   }

   void vic2_656x_t::IoControllerT::write(Address address, Data data)
   {
      address &= 0x3f;
      if (address >= 0x30)
         return;
      *((Data*)&_chip._regs + address) = data;
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

	static const types::Colour vic2_colours[] = {
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

	class VicII
	{
	public:
		VicII(vic2_config_t config);
		void generate_frame(IHost::Frame::scan_out_t pixels);

	protected:
		void generate_scanline(IHost::Frame::scan_out_t& pixels);

	private:
		vic2_config_t _config;
	};

	VicII::VicII(vic2_config_t config)
		 : _config(config) {
	}

	void VicII::generate_frame(IHost::Frame::scan_out_t pixels) {
		while (!pixels.isEndOfFrame())
			generate_scanline(pixels);
	}

	void VicII::generate_scanline(IHost::Frame::scan_out_t& pixels) {
		for (uint16_t cycle_x = 0; cycle_x < _config._cycles_per_scanline; ++cycle_x) {
		}
	}

	void vic2_656x_t::generateFrame(IHost::Frame& frame) {
		const auto& config = getConfig();
		bool scanline_visible = false;
		bool raster_visible = false;

		auto output_rows = frame._pixels.rows().begin();
		types::Colour* current_pixel = nullptr;

		bool main_border_flip_flop = true;
		bool vert_border_flip_flop = true;
		bool bad_line_enable = false;
		int vc_base = 0; // (video counter base) is a 10 bit data register with reset input that can be loaded with the value from VC.
		int vc = 0; //  (video counter) is a 10 bit counter that can be loaded with the value from VCBASE.
		int vmli = 0; // 6 bit counter with reset input that keeps track of the position within the internal 40×12 bit video matrix / color line where read character pointers are stored resp.read again.
		int rc = 0; // (row counter) is a 3 bit counter with reset input.
		bool bus_available = true;
		uint16_t video_matrix[40]{ 0 };
		bool is_display_state = false;

		(void)bus_available;
		(void)video_matrix;

		for (uint16_t raster_y = 0; raster_y < config._total_scanlines_per_frame; ++raster_y) {
			_regs._raster = (raster_y & 0xff);
			_regs._control_reg_1._rst8 = (raster_y & 0x100) ? 1 : 0;

			for (uint16_t cycle_x = 0; cycle_x < config._cycles_per_scanline; ++cycle_x) {

				if ((raster_y == 0x30) && _regs._control_reg_1._den)
					bad_line_enable = true;
				else if (raster_y == 0xf8)
					bad_line_enable = false;
				bool is_bad_line = bad_line_enable && ((raster_y & 7) == _regs._control_reg_1._y_scroll);
				if (is_bad_line)
					is_display_state = true;
				
				if (cycle_x == 58) {
					if (rc == 7) {
						is_display_state = false;
						vc_base = vc;
					}
					if (is_display_state)
						rc = (rc + 1) & 7;
				}

				if (cycle_x == 14) {
					vc = vc_base;
					vmli = 0;
					if (is_bad_line)
						rc = 0;
				}

				if ((cycle_x >= 12) && (cycle_x <= 54)) {
					if (is_bad_line)
						bus_available = false;	// mwToDo: CPU needs to stop on first read access after BA goes low.
					// mwToDo
					///if (!bus_available && (cycle_x >= 15))
					///	video_matrix[vmli] = mem_read_c();
				}

				if (is_display_state) {
					// mwToDo: Do g-access
					vc = (vc + 1) & 0x3ff;
					vmli = (vmli + 1) & 0x3f;
				}

				_cpu_clock.tick();
				for (uint16_t cell_x = 0; cell_x < 8; ++cell_x) {
					auto raster_x = cell_x | (cycle_x << 3);

					if (raster_x == config._xpos_raster_irq) {
						// mwToDo: Fire raster irq.
						if (raster_y == borderBottomCompare()) // border #2
							vert_border_flip_flop = true;
						if ((raster_y == borderTopCompare()) && _regs._control_reg_1._den)	// border #3
							vert_border_flip_flop = false;
						if (scanline_visible) {
							++output_rows;
						}
						if (raster_y == config._raster_display_on)
							scanline_visible = true;
						else if (raster_y == config._raster_display_off)
							scanline_visible = false;
						current_pixel = (*output_rows).data();
					}

					if (raster_x == config._xpos_display_on)
						raster_visible = scanline_visible;
					else if (raster_x == config._xpos_display_off)
						raster_visible = false;

					if (raster_x == borderRightCompare())	// border #1
						main_border_flip_flop = true;
					if (raster_x == borderLeftCompare()) {
						if (raster_y == borderBottomCompare())	// border #4
							vert_border_flip_flop = true;
						if ((raster_y == borderTopCompare()) && _regs._control_reg_1._den) // border #5
							vert_border_flip_flop = false;
						if (!vert_border_flip_flop)
							main_border_flip_flop = false;
					}


					if (raster_visible) {
						auto pixel_colour = vic2_colours[main_border_flip_flop ? (uint8_t)_regs._border_color : 0];
						//pixel_colour.r = vc & 0xff;
						//pixel_colour.r = rc << 5;
						//pixel_colour.r = vmli;
						*current_pixel++ = pixel_colour;
					}

				}

			}

		}

		// tick some clock cycles...
		//for (auto& row : frame._pixels.rows()) {
		//	for (auto& pixel : row) {
		//		pixel = vic2_colours[14];
		//	}
		//}
	}

	async::Future<> vic2_656x_t::runGpu(IHost& host)
   {

		auto& config = co_await host.events.initialising;
		const auto& vic_config = getConfig();
		config.display_size = {
			._width = gfx::Image::Width(vic_config.visibleScanlineWidth()),
			._height = gfx::Image::Height(vic_config.visibleScanlineCount())
		};

		//VicII vic2(vic_config);
		for (;;) {
			auto& frame = co_await host.events.need_frame;
			generateFrame(frame);
			//vic2.generate_frame(frame);
		}
   }

}
