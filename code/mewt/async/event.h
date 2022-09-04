
#pragma once

#include "mewt/types/enum_traits.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/intrusive_stack.h"
#include "mewt/types/flags.h"

#include <exception>

namespace mewt::async {

	template <typename _EventData>
	class event_t {
	public:
		void dispatch(_EventData& event_data) {
			_event_data = &event_data;
			auto dispatch_stack = std::move(_handlers);
			while (!dispatch_stack.is_empty()) {
				auto& handler = *dispatch_stack.pop();
				handler._continuation();
			}
			_event_data = nullptr;
		}

	protected:
		struct handler_t;
		_EventData* _event_data = nullptr;
		intrusive_stack<handler_t> _handlers;

		struct handler_t : public intrusive_stack<handler_t>::node_t {
			handler_t(event_t& event) : _event(event) {}
			inline bool await_ready() { return false; }
			inline bool await_suspend(std::coroutine_handle<> continuation) {
				_continuation = continuation;
				_event._handlers.push(*this);
				return true;
			}
			inline decltype(auto) await_resume() {
				if (!this->_event._event_data)
					throw std::exception();
				return *this->_event._event_data;
			}
			event_t& _event;
			std::coroutine_handle<> _continuation;
		};

	public:
		inline auto operator co_await() { return handler_t(*this); }
	};

}
