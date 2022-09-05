
#include "mewt/emu/chip/clock/clock.h"

namespace mewt::emu::chip
{

   void clock_source_t::run() {
		while (!_awaiters.is_empty())
			tick();
	}

   void clock_source_t::tick() {
		auto prev_awaiters = std::move(_awaiters);
		while (auto* awaiter = prev_awaiters.pop()) {
			if (--awaiter->_tick_count._tick_count == 0)
				awaiter->_continuation.resume();
			else
				_awaiters.push(*awaiter);
		}
	}

}
