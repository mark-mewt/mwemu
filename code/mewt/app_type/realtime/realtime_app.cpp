
#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/async/future_promise.h"
#include "mewt/async/generator.h"

namespace mewt::app_type::realtime {


	realtime_app_t::realtime_app_t(phase_manager_t& phase_manager)
		 : _phase_manager(phase_manager) {
	}

	auto realtime_app_t::runCore()
		 -> async::future<>
	{

		initApp();

		state_t app_state;
		_phase_manager._event_dispatch.dispatch(PhaseType::Init, app_state);

		auto frame_generator = generateFrames(app_state.renderer());

		for (;;) {
			co_await _event_manager.pump();

			_phase_manager._event_dispatch.dispatch(PhaseType::Update, app_state);
			_phase_manager._event_dispatch.dispatch(PhaseType::Render, app_state);
			co_await frame_generator;
		}
	}

	static auto verticalSync()
		 -> async::future<>
	{
		// SDL_Delay(1000 / 60);
		co_await std::suspend_never();
	}

	auto realtime_app_t::generateFrames(ext::sdl::renderer_t& sdl_renderer)
		 -> async::generator<int>
	{

		for (;;) {
			// clears the screen
			sdl_renderer.clear_screen();

			co_yield 0; // mwToDo: Return some info about the frame.

			// triggers the double buffers
			// for multiple rendering
			sdl_renderer.present_screen();

			// calculates to 60 fps
			co_await verticalSync(); // mwToDo: We may be able to get rid of this, assuming present above will always wait for vsync??
		}
	}

}
