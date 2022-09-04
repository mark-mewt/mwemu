
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/app/c64_emu/c64_emu_host.h"

namespace mewt::app::c64_emu {

	class c64_emu_t : public app_type::realtime::realtime_app_t {
	protected:
		using realtime_app_t::realtime_app_t;
		void init_app() override final;

	private:

		c64_emu_host_t _host{ *this };

	};

}
