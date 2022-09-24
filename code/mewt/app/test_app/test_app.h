
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"

namespace mewt::app::test_app
{

	class TestApp : public app_type::realtime::realtime_app_t
	{
	protected:
		using realtime_app_t::realtime_app_t;
		void initApp() final;

	private:
		auto runInputLoop()
			 -> async::Future<>;

		auto runUpdateLoop()
			 -> async::Future<>;

		auto runRenderer()
			 -> async::Future<>;

		struct SharedState;
		SharedState* _shared_state = nullptr;
	};

}
