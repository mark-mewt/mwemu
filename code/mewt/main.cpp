
#include "mewt/app/c64_emu/c64_emu.h"
#include "mewt/app/test_app/test_app.h"
#include "mewt/app_type/realtime/realtime_app.impl.h"

namespace mewt
{

	auto mewtMain(const os::app_context_i& app_context) -> int
	{
		// sdl::event_dispatch_t event_dispatch;

#if 1
		runApp(app_context, types::traits::kTypeId<app::c64_emu::c64_emu_t>);
#else
		runApp(app_context, types::traits::kTypeId<app::test_app::test_app_t>);
#endif

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
