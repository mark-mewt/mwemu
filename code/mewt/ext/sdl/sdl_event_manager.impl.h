
#pragma once

#include "mewt/ext/sdl/sdl_event_manager.h"
#include "mewt/async/event_dispatch.impl.h"

namespace mewt::ext::sdl
{

	auto EventManager::keyboardEvent() { return EventDispatch::SingleHandler<EventType::Keyboard>(_event_dispatch); }

}
