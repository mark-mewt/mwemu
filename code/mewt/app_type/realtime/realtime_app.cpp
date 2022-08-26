
#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/async/future_promise.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/app_type/realtime/realtime_app_phase.h"
#include "mewt/async/generator.h"

namespace mewt::app_type::realtime {


	realtime_app_t::realtime_app_t(phase_manager_t& phase_manager)
		 : _phase_manager(phase_manager) {
	}

	async::future<> realtime_app_t::run_core() {
		using namespace ext::sdl;
		state_t app_state;

		init_app(app_state);

		_phase_manager._event_dispatch.dispatch(phase_manager_t::phase_type_t::Init, app_state);

		auto frame_generator = generate_frames(app_state.renderer());

		for (;;) {
			co_await _event_manager.pump();

			_phase_manager._event_dispatch.dispatch(phase_manager_t::phase_type_t::Update, app_state);
			_phase_manager._event_dispatch.dispatch(phase_manager_t::phase_type_t::Render, app_state);
			co_await frame_generator;
		}
	}

	static async::future<> vertical_sync() {
		// SDL_Delay(1000 / 60);
		co_await std::suspend_never();
	}

	async::generator<int> realtime_app_t::generate_frames(ext::sdl::renderer_t& sdl_renderer) {

		for (;;) {
			// clears the screen
			sdl_renderer.clear_screen();

			co_yield 42; // mwToDo: Return some info about the frame.

			// triggers the double buffers
			// for multiple rendering
			sdl_renderer.present_screen();

			// calculates to 60 fps
			co_await vertical_sync(); // mwToDo: We may be able to get rid of this, assuming present above will always wait for vsync??
		}
	}

}
