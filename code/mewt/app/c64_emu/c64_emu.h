
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/emu/sys/c64/c64.h"

namespace mewt::app::c64_emu {

	class c64_emu_t : public app_type::realtime::realtime_app_t {
	protected:
		using realtime_app_t::realtime_app_t;
		void init_app(const init_state_t& init_state) override final;

	private:
		async::future<> run_input();
		async::future<> run_updater();
		async::future<> run_renderer();

		emu::sys::c64::c64_t _c64;

	};

}
