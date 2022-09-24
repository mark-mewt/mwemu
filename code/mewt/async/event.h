
#pragma once

#include "mewt/types/intrusive_stack.h"

namespace mewt::async {

	template <typename TEventData>
	class Event
	{
	public:
		inline void dispatch(TEventData& event_data);

	private:
		class Handler;
		TEventData* _event_data = nullptr;
		IntrusiveStack<Handler> _handlers;

	public:
		inline auto operator co_await();
	};
}
