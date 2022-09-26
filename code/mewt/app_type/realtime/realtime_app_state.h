
#pragma once

#include "mewt/app_type/realtime/realtime_app_init.h"
#include "mewt/ext/sdl/sdl_engine.h"
#include "mewt/ext/sdl/sdl_window.h"
#include "mewt/ext/sdl/sdl_renderer.h"

namespace mewt::app_type::realtime
{

	class realtime_app_t::state_t : public init_state_t
	{
	public:
		state_t();
		inline ext::sdl::renderer_t& renderer() override final { return _sdl_renderer; }
		inline const ext::sdl::renderer_t& renderer() const override final { return _sdl_renderer; }

	private:
		ext::sdl::Engine _sdl_engine;
		ext::sdl::window_t _sdl_window;
		ext::sdl::renderer_t _sdl_renderer;
	};


}
