
#pragma once

#include "mewt/types/coroutine.h"
#include "mewt/types/enum_traits.h"
#include "mewt/types/flags.h"
#include "mewt/types/intrusive_stack.h"

#include <exception>

namespace mewt::async {

	template <typename _EventData>
	class EventT
	{
	public:
		void dispatch(_EventData& event_data) {
			_event_data = &event_data;
			auto dispatch_stack = std::move(_handlers);
			while (!dispatch_stack.isEmpty()) {
				auto& handler = *dispatch_stack.pop();
				handler._continuation();
			}
			_event_data = nullptr;
		}

	protected:
		struct handler_t;
		_EventData* _event_data = nullptr;
		IntrusiveStack<handler_t> _handlers;

		struct handler_t : public IntrusiveStack<handler_t>::Node {
			explicit handler_t(EventT& event) : _event(event) {}
			inline auto await_ready() -> bool { return false; }
			inline auto await_suspend(std::coroutine_handle<> continuation) -> bool
			{
				_continuation = continuation;
				_event._handlers.push(*this);
				return true;
			}
			inline auto await_resume() -> decltype(auto)
			{
				if (!this->_event._event_data)
					throw std::exception();
				return *this->_event._event_data;
			}
			EventT& _event;
			std::coroutine_handle<> _continuation;
		};

	public:
		inline auto operator co_await() { return handler_t(*this); }
	};
}
