
#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/async/future_promise.h"
#include "mewt/async/event_dispatch.impl.h"

#include "SDL/SDL_events.h"

namespace mewt::ext::sdl {

	async::Future<> EventManager::pump() {
		SDL_Event current_event;
		while (SDL_PollEvent(&current_event)) {
			if (current_event.type == SDL_QUIT)
				co_await std::suspend_always();
			_event_dispatch.dispatch(EventType::Keyboard, current_event);
		}
	}

	int Event::sdl_event_type() const {
		return _sdl_event.type;
	}

	keyboard::Scancode KeyboardEvent::scancode() const {
		return static_cast<keyboard::Scancode>(_sdl_event.key.keysym.scancode);
	}

}
