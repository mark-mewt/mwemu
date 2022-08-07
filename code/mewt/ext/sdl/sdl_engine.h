
#pragma once

#include "mewt/types/flags.h"
#include "mewt/types/non_movable.h"

namespace mewt::ext::sdl {

	enum class subsystem_t {
		Timer = 0,
		Audio = 4,
		Video = 5,
		Joystick = 9,
		Haptic = 12,
		GameController = 13,
		Events = 14,
		Sensor = 15,
	};

	using subsystems_t = types::flags<subsystem_t>;
	using types::operator|;

	class engine_t : types::non_movable_t {

	public:
		explicit engine_t(subsystems_t subsystems);

		void start_subsystems(subsystems_t subsystems);
		void stop_subsystems(subsystems_t subsystems);

		subsystems_t query_started_subsystems(subsystems_t query_mask = types::no_flags_t{});

		~engine_t();
	};

}
