
#include "mewt/ext/sdl/sdl_main_loop.h"

#include <string>
#include "SDL/SDL_events.h"

namespace mewt::ext::sdl {

	main_loop_t::main_loop_t(event_dispatch_t& event_dispatch) 
	: _event_dispatch(event_dispatch) {
	}

	main_loop_t::iterator_t main_loop_t::begin() {
		return { *this };
	}

	main_loop_t::sentinel_t main_loop_t::end() {
		return { };
	}

	bool main_loop_t::is_active() const { return _is_active; }

	void main_loop_t::frame_advance() {
		SDL_Event event;

		// todo: split into frame start, for the event loop, and frame end for frame present etc.
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				_is_active = false;
			_event_dispatch.dispatch_sdl(event);
		}

	}

}
