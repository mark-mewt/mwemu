
#pragma once

#include "mewt/app_type/realtime/realtime_app_phase.h"

namespace mewt::app_type::realtime {

	template <std::derived_from<realtime_app_t> _App>
	void run_app(const os::app_context_i& app_context, types::class_id<_App>) {
		realtime_app_t::phase_manager_t phase_manager;
		_App app(phase_manager);
		auto app_coro = app.run_core();
	}

}
