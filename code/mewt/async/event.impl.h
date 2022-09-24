
#pragma once

#include "mewt/async/event.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/enum_traits.h"
#include "mewt/types/intrusive_stack.impl.h"

#include <exception>

namespace mewt::async
{

	template <typename TEventData>
	void Event<TEventData>::dispatch(TEventData& event_data)
	{
		_event_data = &event_data;
		auto dispatch_stack = std::move(_handlers);
		while (!dispatch_stack.isEmpty())
		{
			auto& handler = *dispatch_stack.pop();
			handler._continuation();
		}
		_event_data = nullptr;
	}

	template <typename TEventData>
	class Event<TEventData>::Handler : public IntrusiveStack<Handler>::Node
	{
	public:
		explicit Handler(Event& event) : _event(event) {}
		inline auto await_ready() // NOLINT(readability-identifier-naming)
			 -> bool
		{
			return false;
		}
		inline auto await_suspend(std::coroutine_handle<> continuation) // NOLINT(readability-identifier-naming)
			 -> bool
		{
			_continuation = continuation;
			_event._handlers.push(*this);
			return true;
		}
		inline auto await_resume() // NOLINT(readability-identifier-naming)
			 -> decltype(auto)
		{
			if (!this->_event._event_data)
				throw std::exception();
			return *this->_event._event_data;
		}

	private:
		Event& _event;
		std::coroutine_handle<> _continuation;
		friend Event;
	};

	template <typename TEventData>
	auto Event<TEventData>::operator co_await() { return Handler(*this); }

}
