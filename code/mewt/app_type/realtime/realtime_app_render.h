
#pragma once

#include "mewt/app_type/realtime/realtime_app_update.h"

namespace mewt::app_type::realtime {

	class realtime_app_t::render_state_t : public update_state_t {
	public:
		virtual ext::sdl::renderer_t& renderer() = 0;
		virtual const ext::sdl::renderer_t& renderer() const = 0;
	};

}
