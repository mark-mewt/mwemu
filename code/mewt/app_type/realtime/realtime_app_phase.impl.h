
#pragma once

#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/async/event_dispatch.impl.h"

namespace mewt::app_type::realtime
{

	template <realtime_app_t::PhaseType NPhase>
	auto realtime_app_t::phase_manager_t::phase() { return EventDispatch::SingleHandler<NPhase>(_event_dispatch); }

	inline auto realtime_app_t::initPhase()
	{
		return _phase_manager.phase<PhaseType::Init>();
	}

	inline auto realtime_app_t::updatePhase()
	{
		return _phase_manager.phase<PhaseType::Update>();
	}

	inline auto realtime_app_t::renderPhase()
	{
		return _phase_manager.phase<PhaseType::Render>();
	}

}
