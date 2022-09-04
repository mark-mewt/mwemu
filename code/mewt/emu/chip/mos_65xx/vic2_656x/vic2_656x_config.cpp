
#include "mewt/emu/chip/mos_65xx/vic2_656x/vic2_656x_config.h"

#include <exception>

namespace mewt::emu::chip::mos_65xx {
	/*
	template<vic2_model_t _Model>
	struct vic2_model_info;

	template <>
	struct vic2_model_info<vic2_model_t::VIC2_6567R56A> {
		static constexpr vic2_config_t config = {
			._video_standard = gfx::video_output::standard_t::NTSC_M,
			._cycles_per_scanline = 64,
			._total_scanlines_per_frame = 262,
			._raster_display_on = 41,
			._raster_display_off = 13,
			._xpos_raster_irq = 412,
			._xpos_display_on = 488,
			._xpos_display_off = 387,//388,
		};
	};

	template <>
	struct vic2_model_info<vic2_model_t::VIC2_6567R8> {
		static constexpr vic2_config_t config = {
			._video_standard = gfx::video_output::standard_t::NTSC_M,
			._cycles_per_scanline = 65,
			._total_scanlines_per_frame = 263,
			._raster_display_on = 41,
			._raster_display_off = 13,
			._xpos_raster_irq = 412,
			._xpos_display_on = 489,
			._xpos_display_off = 387,//396,
		};
	};

	template <>
	struct vic2_model_info<vic2_model_t::VIC2_6569> {
		static constexpr vic2_config_t config = {
			._video_standard = gfx::video_output::standard_t::NTSC_M,
			._cycles_per_scanline = 63,
			._total_scanlines_per_frame = 312,
			._raster_display_on = 16,
			._raster_display_off = 300,
			._xpos_raster_irq = 404,
			._xpos_display_on = 480,
			._xpos_display_off = 379,//380,
		};
	};

	template <vic2_model_t _Model>
	constexpr const vic2_config_t& vic2_config = vic2_model_info<_Model>::config;

	template <auto _LHS, auto _RHS>
	struct compare {
		static constexpr bool equal() { return _LHS == _RHS; }
	};

	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R56A>.total_pixels_per_scanline(), 512>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R8>.total_pixels_per_scanline(), 520>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6569>.total_pixels_per_scanline(), 504>::equal(), "Config fail!");

	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R56A>.visible_scanline_count(), 234>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R8>.visible_scanline_count(), 235>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6569>.visible_scanline_count(), 284>::equal(), "Config fail!");

	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R56A>.visible_scanline_width(), 411>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6567R8>.visible_scanline_width(), 418>::equal(), "Config fail!");
	static_assert(compare<vic2_config<vic2_model_t::VIC2_6569>.visible_scanline_width(), 403>::equal(), "Config fail!");

	const vic2_config_t& vic2_config_t::get(vic2_model_t model) {
		switch (model) {
		case vic2_model_t::VIC2_6567R56A:
			return vic2_config<vic2_model_t::VIC2_6567R56A>;
		case vic2_model_t::VIC2_6567R8:
			return vic2_config<vic2_model_t::VIC2_6567R8>;
		case vic2_model_t::VIC2_6569:
			return vic2_config<vic2_model_t::VIC2_6569>;
		}
		throw std::exception("Unknown Vic-II model.");
	}*/

}
