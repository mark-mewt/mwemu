
#pragma once

#include "mewt/types/enum_traits.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/intrusive_stack.h"
#include "mewt/types/flags.h"

#include <exception>

namespace mewt::async {

	template <typename _EventData, typename _EventTypes, template <_EventTypes> class _DataType>
	class event_dispatch_t {
	public:
		using event_types_t = types::flags<_EventTypes>;
		void dispatch(event_types_t event_types, const _EventData& event_data) {
			_event_data = &event_data;
			auto dispatch_stack = std::move(_handlers);
			while (!dispatch_stack.is_empty()) {
				auto& handler = *dispatch_stack.pop();
				if (handler._event_types.intersect(event_types).is_empty())
					_handlers.push(handler);
				else
					handler._continuation();
			}
			_event_data = nullptr;
		}

	protected:
		const _EventData* _event_data = nullptr;
		struct handler_t;
		intrusive_stack<handler_t> _handlers;
		struct handler_t : public intrusive_stack<handler_t>::node_t {
			handler_t(event_dispatch_t& event_dispatch, event_types_t event_types) : _event_dispatch(event_dispatch), _event_types(event_types) {}
			inline bool await_ready() { return false; }
			inline bool await_suspend(std::coroutine_handle<> continuation) {
				_continuation = continuation;
				_event_dispatch._handlers.push(*this);
				return true;
			}
			event_dispatch_t& _event_dispatch;
			event_types_t _event_types;
			std::coroutine_handle<> _continuation;
		};

	public:
		template <_EventTypes _Event>
		struct single_handler_t : public handler_t {
			inline single_handler_t(event_dispatch_t& event_dispatch) : handler_t(event_dispatch, _Event) {}
			inline decltype(auto) await_resume() {
				if (!this->_event_dispatch._event_data)
					throw std::exception();
				return static_cast<typename _DataType<_Event>::type>(*this->_event_dispatch._event_data);
			}
		};
	};

}

