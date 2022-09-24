
#pragma once

#include "mewt/app_type/realtime/realtime_app_phase.h"

namespace mewt::app_type::realtime {

	template <std::derived_from<realtime_app_t> TApp>
	void runApp(const os::app_context_i& /*app_context*/, types::ClassId<TApp> /*unused*/)
	{
		realtime_app_t::phase_manager_t phase_manager;
		TApp app(phase_manager);
		app.runCoreUntilSuspend();
	}

}
