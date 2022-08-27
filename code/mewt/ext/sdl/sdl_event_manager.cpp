
#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/async/future_promise.h"

#include "SDL/SDL_events.h"

namespace mewt::ext::sdl {

	async::future<> event_manager_t::pump() {
		SDL_Event current_event;
		while (SDL_PollEvent(&current_event)) {
			if (current_event.type == SDL_QUIT)
				co_await std::suspend_always();
			_event_dispatch.dispatch(event_type_t::Keyboard, current_event);
		}
	}

	int event_t::sdl_event_type() const {
		return _sdl_event.type;
	}

	keyboard::scancode_t keyboard_event_t::scancode() const {
		return static_cast<keyboard::scancode_t>(_sdl_event.key.keysym.scancode);
	}

}