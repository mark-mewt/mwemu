
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/ext/sdl/sdl_image.h"

namespace mewt::app::test_app
{

	class TestApp : public app_type::realtime::realtime_app_t
	{
	protected:
		using realtime_app_t::realtime_app_t;
		void initApp() final;

	private:
		auto runInputLoop()
			 -> async::future<>;

		auto runUpdateLoop()
			 -> async::future<>;

		auto runRenderer()
			 -> async::future<>;

		ext::sdl::image_t::rect_t _rect;
		ext::sdl::image_t::rect_t _output_bounds;
		enum class Keypress
		{
			Up,
			Down,
			Left,
			Right
		};
		using Keypresses = types::flags<Keypress>;
		Keypresses _keypresses{ 0 };
	};

}
