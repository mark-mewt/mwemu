
#pragma once

#include "mewt/types/flags.h"

namespace mewt::ext::sdl
{

	enum class Subsystem
	{
		Timer = 0,
		Audio = 4,
		Video = 5,
		Joystick = 9,
		Haptic = 12,
		GameController = 13,
		Events = 14,
		Sensor = 15,
	};

	using Subsystems = types::flags<Subsystem>;
	using types::operator|;

	class Engine
	{

	public:
		explicit Engine(Subsystems subsystems);

		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		auto operator=(const Engine&) = delete;
		auto operator=(Engine&&) = delete;

		static void startSubsystems(Subsystems subsystems);
		static void stopSubsystems(Subsystems subsystems);

		[[nodiscard]] static auto queryStartedSubsystems(Subsystems query_mask = types::NoFlags())
			 -> Subsystems;

		~Engine();
	};

}
