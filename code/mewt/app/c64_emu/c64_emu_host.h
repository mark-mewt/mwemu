
#pragma once

#include "mewt/app_type/realtime/realtime_app.decl.h"
#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/host/host.h"
#include "mewt/emu/sys/c64/c64.decl.h"
#include "mewt/emu/sys/system.h"

#include <memory>

namespace mewt::app::c64_emu
{

	class EmulatorHost final : public emu::IHost
	{

	public:
		explicit EmulatorHost(app_type::realtime::realtime_app_t& app);

		//~EmulatorHost() final;

	protected:
		void initHost() final;

	private:
		auto runInput()
			 -> async::Future<>;

		auto runUpdater()
			 -> async::Future<>;

		auto runRenderer()
			 -> async::Future<>;

		app_type::realtime::realtime_app_t& _app;
		std::unique_ptr<emu::ISystem> _c64;
	};

}
