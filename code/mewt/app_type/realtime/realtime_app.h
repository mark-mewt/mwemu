
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/async/generator.decl.h"
#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/ext/sdl/sdl_renderer.decl.h"
#include "mewt/os/app_context/app_context.decl.h"

#include "mewt/types/traits.h"

namespace mewt::app_type::realtime {

	class realtime_app_t {
	public:
		class init_state_t;
		class render_state_t;
		class update_state_t;
		class phase_manager_t;
		class state_t;

		async::future<> run_core();

		template <std::derived_from<realtime_app_t> _App>
		inline friend void run_app(const os::app_context_i& app_context, types::class_id<_App>);

	protected:
		virtual void init_app(const init_state_t& sdl_renderer) = 0;
		inline phase_manager_t& phase_manager() { return _phase_manager; }
		inline ext::sdl::event_manager_t& event_manager() { return _event_manager; }

	private:

		realtime_app_t(phase_manager_t& phase_manager);

		phase_manager_t& _phase_manager;
		ext::sdl::event_manager_t _event_manager;

		async::generator<int> generate_frames(ext::sdl::renderer_t& sdl_renderer);
	};

}
