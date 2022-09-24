
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"

namespace mewt::app_type::realtime {

	enum class realtime_app_t::PhaseType
	{
		Init,
		Update,
		Render
	};

	class realtime_app_t::phase_manager_t {
	public:
		template <PhaseType NPhase>
		struct event_data_type;

		template <>
		struct event_data_type<PhaseType::Init> { using Type = const init_state_t&; };
		template <>
		struct event_data_type<PhaseType::Update> { using Type = const update_state_t&; };
		template <>
		struct event_data_type<PhaseType::Render> { using Type = const render_state_t&; };

		template <PhaseType NPhase>
		constexpr friend auto getEventDataType(const state_t& event_data)
			 -> typename event_data_type<NPhase>::Type
		{
			return event_data;
		}

		using EventDispatch = async::EventDispatch<state_t, PhaseType, event_data_type>;
		EventDispatch _event_dispatch;

		template <PhaseType NPhase>
		auto phase() { return EventDispatch::SingleHandler<NPhase>(_event_dispatch); }
		// auto render_phase() { return event_dispatch_t::single_handler_t<phase_type_t::Render>(_event_dispatch); }
	};



}
