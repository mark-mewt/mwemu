
#pragma once

#include "mewt/app_type/realtime/realtime_app.decl.h"
#include "mewt/async/future_promise.decl.h"
#include "mewt/async/generator.decl.h"
#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/ext/sdl/sdl_renderer.decl.h"
#include "mewt/os/app_context/app_context.decl.h"
#include "mewt/types/interface.h"
#include "mewt/types/traits.h"
#include "mewt/types/traits/traits.TypeId.h"

namespace mewt::app_type::realtime
{

	class realtime_app_t : public types::IInterface
	{
	public:
		class init_state_t;
		class render_state_t;
		class update_state_t;
		class phase_manager_t;
		enum class PhaseType;
		class state_t;

		auto runCore() -> async::Future<>;
		void runCoreUntilSuspend();

		template <std::derived_from<realtime_app_t> TApp>
		inline friend void runApp(const os::app_context_i& app_context,
										  types::traits::TypeId<TApp> /*unused*/);

		inline auto phaseManager() -> phase_manager_t&
		{
			return _phase_manager;
		}

		inline auto eventManager() -> ext::sdl::EventManager&
		{
			return _event_manager;
		}

		inline auto initPhase();
		inline auto updatePhase();
		inline auto renderPhase();

	protected:
		virtual void initApp() = 0;

	private:
		explicit realtime_app_t(phase_manager_t& phase_manager);

		phase_manager_t& _phase_manager;
		ext::sdl::EventManager _event_manager;

		static auto generateFrames(ext::sdl::renderer_t& sdl_renderer) -> async::Generator<int>;
	};

}
