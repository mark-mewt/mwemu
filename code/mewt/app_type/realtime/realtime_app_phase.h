
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
		template <PhaseType _Phase>
		struct event_data_type;

		template <>
		struct event_data_type<PhaseType::Init> { using type = const init_state_t&; };
		template <>
		struct event_data_type<PhaseType::Update> { using type = const update_state_t&; };
		template <>
		struct event_data_type<PhaseType::Render> { using type = const render_state_t&; };

		template <PhaseType _Phase>
		constexpr friend typename event_data_type<_Phase>::type get_event_data_type(const state_t& event_data) {
			return event_data;
		}

		using event_dispatch_t = async::event_dispatch_t<state_t, PhaseType, event_data_type>;
		event_dispatch_t _event_dispatch;

		template <PhaseType _Phase>
		auto phase() { return event_dispatch_t::single_handler_t<_Phase>(_event_dispatch); }
		// auto render_phase() { return event_dispatch_t::single_handler_t<phase_type_t::Render>(_event_dispatch); }
	};



}
