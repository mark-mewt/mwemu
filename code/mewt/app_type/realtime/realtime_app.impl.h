
#pragma once

#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/async/future_promise.h"

namespace mewt::app_type::realtime {

	template <std::derived_from<realtime_app_t> TApp>
	void runApp(const os::app_context_i& /*app_context*/, types::ClassId<TApp> /*unused*/)
	{
		realtime_app_t::phase_manager_t phase_manager;
		TApp app(phase_manager);
		auto app_coro = app.runCore();
		// mwToDo: The coro will only run until the first suspend point, and then it will bail.
		// This means if it co_awaits any event that is external to this thread, this will exit.
		// Ultimately, we want a scheduler here that will allow continuations to be scheduled
		// back on this thread after such an external event occurs, while we meanwhile wait for
		// an exit signal to do the real exit.
	}

	inline auto realtime_app_t::initPhase() {
		return _phase_manager.phase<PhaseType::Init>();
	}

	inline auto realtime_app_t::updatePhase() {
		return _phase_manager.phase<PhaseType::Update>();
	}

	inline auto realtime_app_t::renderPhase() {
		return _phase_manager.phase<PhaseType::Render>();
	}

}
