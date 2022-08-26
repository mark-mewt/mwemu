
#include "mewt/app/test_app/test_app.h"
#include "mewt/app_type/realtime/realtime_app.impl.h"


#include "mewt/emu/sys/c64/c64.h"

#include "mewt/os/app_context/app_context.h"
#include "mewt/gui/window_manager/window_manager.h"
#include "mewt/gui/window/window.h"
#include "mewt/ext/sdl/sdl_engine.h"
#include "mewt/ext/sdl/sdl_window.h"
#include "mewt/ext/sdl/sdl_renderer.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/types/scale_factor.h"
#include "mewt/async/future_promise.h"
#include "mewt/async/generator.h"
#include "mewt/diag/log.h"
#include "mewt/types/intrusive_stack.h"
#include "mewt/async/event_dispatch.h"

#include "SDL/SDL_timer.h"

namespace mewt {



	int mewt_main(const os::app_context_i& app_context) {

		// sdl::event_dispatch_t event_dispatch;

		run_app(app_context, types::class_id_of<app::test_app::test_app_t>());



		// gui::window_manager_t window_manager = gui::window_manager_i::create(app_context);
		// gui::window_t window = gui::window_i::create(window_manager);
		// window->run();

		// emu::sys::c64::c64_t c64;
		// c64.run_sys();
		return 0;
	}
}

/*
 *
 * http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/
 *
 *
 */
