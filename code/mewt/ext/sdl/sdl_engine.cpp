
#include "mewt/ext/sdl/sdl_engine.h"
#include "mewt/ext/sdl/sdl_error.impl.h"

#include <SDL/SDL.h>
#include <string>

namespace mewt::ext::sdl
{

	Engine::Engine(Subsystems subsystems)
	{
		checkStatus(SDL_InitSubSystem(subsystems.rawBits()));
	}

	void Engine::startSubsystems(Subsystems subsystems)
	{
		checkStatus(SDL_InitSubSystem(subsystems.rawBits()));
	}

	void Engine::stopSubsystems(Subsystems subsystems)
	{
		SDL_QuitSubSystem(subsystems.rawBits());
	}

	auto Engine::queryStartedSubsystems(Subsystems query_mask)
		 -> Subsystems
	{
		return Subsystems(SDL_WasInit(query_mask.rawBits()));
	}

	Engine::~Engine()
	{
		SDL_Quit();
	}

}
