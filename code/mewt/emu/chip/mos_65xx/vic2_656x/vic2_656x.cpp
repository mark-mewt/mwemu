
#include "mewt/async/event.impl.h"
#include "mewt/async/future_promise.h"
#include "mewt/diag/log.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x.h"
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"
#include "mewt/emu/host/host.impl.h"

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

	// gfx::image_t::size_t vic2_656x_t::display_size() const {
	//	return gfx::image_t::size_t{
	//		._width = gfx::image_t::width_t(_config.visible_scanline_width()),
	//		._height = gfx::image_t::height_t(_config.visible_scanline_count())
	//	};
	// }

	constexpr int kVicIIRegisterAddressBits = 6;
	constexpr Address kVicIIRegisterAddressMask = Address((1 << kVicIIRegisterAddressBits) - 1);
	constexpr Data kVicIIRegisterOutOfBoundsValue = Data(0xff);

	auto vic2_656x_t::IOController::read(Address address) -> Data
	{
		// https://www.c64-wiki.com/wiki/Page_208-211
		address &= kVicIIRegisterAddressMask;
		if (address >= Address(sizeof(Regs)))
			return kVicIIRegisterOutOfBoundsValue;
		auto reg_data = types::ByteSpan(_chip._regs);
		// auto reg_data = std::span<Regs, 1>(std::addressof(_chip._regs), 1); // #mwToDo: Store reg_data as a member of IOController
		return reg_data[lowBits<6>(address)];
		//  #mwToDo: Should we use std::byte instead of std::uint8_t for memory contents?
		//  return *((Data*)&_chip._regs + address);
	}

	void vic2_656x_t::IOController::write(Address address, Data data)
	{
		address &= kVicIIRegisterAddressMask;
		if (address >= Address(sizeof(Regs)))
			return;
		auto reg_data = types::ByteSpan(_chip._regs);
		reg_data[lowBits<6>(address)] = data;
		// auto reg_data = std::span<Regs, 1>(std::addressof(_chip._regs), 1);
		// std::as_writable_bytes(reg_data)[address] = static_cast<std::byte>(data);
		//*((Data*)&_chip._regs + address) = data;
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

	constexpr auto operator"" _rgb(std::uint64_t value)
	{
		return types::colourFromRGB(static_cast<std::uint32_t>(value));
	}

	static constexpr auto kVicIIColours = std::to_array<types::Colour>({
		 0x000000_rgb, // { 0x00, 0x00, 0x00 },
		 0xffffff_rgb, // { 0xff, 0xff, 0xff },
		 0x9f4e44_rgb, //{ 0x9f, 0x4e, 0x44 },
		 0x6abfc6_rgb,
		 0xa057a3_rgb,
		 0x5cab5e_rgb,
		 0x50459b_rgb,
		 0xc9d487_rgb,
		 0xa1683c_rgb,
		 0x6d5412_rgb,
		 0xcb7e75_rgb,
		 0x626262_rgb,
		 0x898989_rgb,
		 0x9ae29b_rgb,
		 0x887ecb_rgb,
		 0xadadad_rgb,
	});

	class VicII
	{
	public:
		explicit VicII(vic2_config_t config);
		void generateFrame(IHost::Frame::scan_out_t pixels);

	protected:
		void generateScanline(IHost::Frame::scan_out_t& pixels) const;

	private:
		vic2_config_t _config;
	};

	VicII::VicII(vic2_config_t config)
		 : _config(config)
	{
	}

	void VicII::generateFrame(IHost::Frame::scan_out_t pixels)
	{
		while (!pixels.isEndOfFrame())
			generateScanline(pixels);
	}

	void VicII::generateScanline(IHost::Frame::scan_out_t& /*pixels*/) const
	{
		for (uint16_t cycle_x = 0; cycle_x < _config._cycles_per_scanline; ++cycle_x)
		{
		}
	}

	// #mwToDo: Remove the nolint on the following line
	void vic2_656x_t::generateFrame(IHost::Frame& frame) // NOLINT(readability-function-cognitive-complexity)
	{
		const auto& config = getConfig();
		bool scanline_visible = false;
		bool raster_visible = false;

		auto output_rows = begin(frame._pixels.rows());
		types::Colour* current_pixel = nullptr;

		bool main_border_flip_flop = true;
		bool vert_border_flip_flop = true;
		bool bad_line_enable = false;
		int reg_vc_base = 0; // (video counter base) is a 10 bit data register with reset input that can be loaded with the value from VC.
		int reg_vc = 0;		//  (video counter) is a 10 bit counter that can be loaded with the value from VCBASE.
		int reg_vmli = 0;		// 6 bit counter with reset input that keeps track of the position within the internal 40×12 bit video matrix / color line where read character pointers are stored resp.read again.
		int reg_rc = 0;		// (row counter) is a 3 bit counter with reset input.
		bool bus_available = true;
		const std::array<uint16_t, 40> video_matrix{ 0 };
		bool is_display_state = false;

		(void)bus_available;
		(void)video_matrix;

		for (uint16_t raster_y = 0; raster_y < config._total_scanlines_per_frame; ++raster_y)
		{
			_regs._raster = types::lowByte(raster_y);
			_regs._control_reg_1._rst8 = types::highByte(raster_y) == 1;

			for (uint16_t cycle_x = 0; cycle_x < config._cycles_per_scanline; ++cycle_x)
			{

				if ((raster_y == 0x30) && _regs._control_reg_1._den)
					bad_line_enable = true;
				else if (raster_y == 0xf8)
					bad_line_enable = false;
				bool is_bad_line = bad_line_enable && ((raster_y & 7) == _regs._control_reg_1._y_scroll);
				if (is_bad_line)
					is_display_state = true;

				if (cycle_x == 58)
				{
					if (reg_rc == 7)
					{
						is_display_state = false;
						reg_vc_base = reg_vc;
					}
					if (is_display_state)
						reg_rc = (reg_rc + 1) & 7;
				}

				if (cycle_x == 14)
				{
					reg_vc = reg_vc_base;
					reg_vmli = 0;
					if (is_bad_line)
						reg_rc = 0;
				}

				if ((cycle_x >= 12) && (cycle_x <= 54))
				{
					if (is_bad_line)
						bus_available = false; // mwToDo: CPU needs to stop on first read access after BA goes low.
													  // mwToDo
													  /// if (!bus_available && (cycle_x >= 15))
													  ///	video_matrix[vmli] = mem_read_c();
				}

				if (is_display_state)
				{
					// mwToDo: Do g-access
					reg_vc = (reg_vc + 1) & 0x3ff;
					reg_vmli = (reg_vmli + 1) & 0x3f;
				}

				_cpu_clock.tick();
				for (uint16_t cell_x = 0; cell_x < 8; ++cell_x)
				{
					auto raster_x = cell_x | (cycle_x << 3);

					if (raster_x == config._xpos_raster_irq)
					{
						// mwToDo: Fire raster irq.
						if (raster_y == borderBottomCompare()) // border #2
							vert_border_flip_flop = true;
						if ((raster_y == borderTopCompare()) && _regs._control_reg_1._den) // border #3
							vert_border_flip_flop = false;
						if (scanline_visible)
						{
							++output_rows;
						}
						if (raster_y == config._raster_display_on)
							scanline_visible = true;
						else if (raster_y == config._raster_display_off)
							scanline_visible = false;
						current_pixel = scanline_visible ? (*output_rows).data() : nullptr;
					}

					if (raster_x == config._xpos_display_on)
						raster_visible = scanline_visible;
					else if (raster_x == config._xpos_display_off)
						raster_visible = false;

					if (raster_x == borderRightCompare()) // border #1
						main_border_flip_flop = true;
					if (raster_x == borderLeftCompare())
					{
						if (raster_y == borderBottomCompare()) // border #4
							vert_border_flip_flop = true;
						if ((raster_y == borderTopCompare()) && _regs._control_reg_1._den) // border #5
							vert_border_flip_flop = false;
						if (!vert_border_flip_flop)
							main_border_flip_flop = false;
					}


					if (raster_visible)
					{
						auto pixel_colour = kVicIIColours[main_border_flip_flop ? (uint8_t)_regs._border_color : 0];
						// pixel_colour.r = vc & 0xff;
						// pixel_colour.r = rc << 5;
						// pixel_colour.r = vmli;
						*current_pixel++ = pixel_colour;
					}
				}
			}
		}

		// tick some clock cycles...
		// for (auto& row : frame._pixels.rows()) {
		//	for (auto& pixel : row) {
		//		pixel = vic2_colours[14];
		//	}
		//}
	}

	async::Future<> vic2_656x_t::runGpu(IHost& host)
	{

		auto& config = co_await host.events().initialising;
		const auto& vic_config = getConfig();
		config.display_size = {
			._width = gfx::Image::Width(visibleScanlineWidth(vic_config)),
			._height = gfx::Image::Height(visibleScanlineCount(vic_config))
		};

		// VicII vic2(vic_config);
		for (;;)
		{
			auto& frame = co_await host.events().need_frame;
			generateFrame(frame);
			// vic2.generate_frame(frame);
		}
	}

}
