
#include "mewt/ext/sdl/sdl_event_dispatch.h"

#include <string>
#include "SDL/SDL_events.h"

namespace mewt::ext::sdl {

	void event_dispatch_t::dispatch_sdl(SDL_Event& event) const {
		switch (event.type) {
		case SDL_KEYDOWN:
			dispatch_event(events::keyboard_event_t(event));
			break;
		default:
			break;
		}
	}
}
