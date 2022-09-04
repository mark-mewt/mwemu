
#pragma once

#include "mewt/async/event.h"
#include "mewt/gfx/image.h"
#include "mewt/types/colour.h"
#include "mewt/types/span_2d.h"

namespace mewt::emu {

	class host_t {
	public:

		struct config_t {
			gfx::image_t::size_t display_size;
		};

		struct frame_t {
			types::span_2d_t<types::colour_t, gfx::image_t::width_t, gfx::image_t::height_t> _pixels;
		};

		struct events_t {
			async::event_t<config_t> initialising;
			async::event_t<frame_t> need_frame;
		};
		events_t events;

		void run_emu_host();

	protected:

		virtual void init_host() = 0;

		config_t _host_config;

	};

}
