
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/ext/sdl/sdl_image.h"

namespace mewt::app::test_app {

	class test_app_t : public app_type::realtime::realtime_app_t {
	protected:
		using realtime_app_t::realtime_app_t;
		void init_app() override final;

	private:
		async::future<> run_input_loop();
		async::future<> run_update_loop();
		async::future<> run_renderer();
		ext::sdl::image_t::rect_t _rect;
		ext::sdl::image_t::rect_t _output_bounds;
		enum class keypress_t {
			Up,
			Down,
			Left,
			Right
		};
		using keypresses_t = types::flags<keypress_t>;
		keypresses_t _keypresses{ 0 };
	};

}
