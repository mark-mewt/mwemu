
#pragma once

#include "mewt/emu/sys/c64/c64.h"
#include "mewt/emu/host/host.h"
#include "mewt/app_type/realtime/realtime_app.decl.h"

#include <memory>

namespace mewt::app::c64_emu {

	class c64_emu_host_t : public emu::host_t {

	public:

		c64_emu_host_t(app_type::realtime::realtime_app_t& app);

	protected:

		void init_host() override final;

	private:
		async::future<> run_input();
		async::future<> run_updater();
		async::future<> run_renderer();

		app_type::realtime::realtime_app_t& _app;
		std::unique_ptr<emu::sys::c64::c64_t> _c64;

	};

}
