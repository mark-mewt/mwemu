
#pragma once

#include "mewt/app_type/realtime/realtime_app.decl.h"
#include "mewt/emu/host/host.h"
#include "mewt/emu/sys/c64/c64.h"

#include <memory>

namespace mewt::app::c64_emu {

	class EmulatorHost : public emu::IHost
	{

	public:

		explicit EmulatorHost(app_type::realtime::realtime_app_t& app);

	protected:

		void initHost() final;

	private:

		auto runInput()
			-> async::future<>;

		auto runUpdater()
			-> async::future<>;

		auto runRenderer() 
			-> async::future<>;

		app_type::realtime::realtime_app_t& _app;
		std::unique_ptr<emu::sys::c64::c64_t> _c64;

	};

}
