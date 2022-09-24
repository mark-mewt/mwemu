
#pragma once

#include "mewt/types/coroutine.h"
#include "mewt/types/enum_traits.h"
#include "mewt/types/flags.h"
#include "mewt/types/intrusive_stack.h"

#include <exception>

namespace mewt::async {

	template <typename TEventData, typename TEventTypes, template <TEventTypes> class TDataType>
	class EventDispatch {
	public:
		using EventTypes = types::flags<TEventTypes>;
		void dispatch(EventTypes event_types, const TEventData& event_data)
		{
			_event_data = &event_data;
			auto dispatch_stack = std::move(_handlers);
			while (!dispatch_stack.isEmpty()) {
				auto& handler = *dispatch_stack.pop();
				if (handler._event_types.intersect(event_types).isEmpty())
					_handlers.push(handler);
				else
					handler._continuation();
			}
			_event_data = nullptr;
		}

	private:
		const TEventData* _event_data = nullptr;
		class Handler;
		IntrusiveStack<Handler> _handlers;
		class Handler : public IntrusiveStack<Handler>::Node
		{
		public:
			Handler(EventDispatch& event_dispatch, EventTypes event_types) : _event_dispatch(event_dispatch), _event_types(event_types) {}
			inline auto await_ready() // NOLINT(readability-identifier-naming)
				-> bool
			{
				return false;
			}
			inline auto await_suspend(std::coroutine_handle<> continuation) // NOLINT(readability-identifier-naming)
				 -> bool
			{
				_continuation = continuation;
				_event_dispatch._handlers.push(*this);
				return true;
			}

		private:
			friend EventDispatch;
			EventDispatch& _event_dispatch;
			EventTypes _event_types;
			std::coroutine_handle<> _continuation;
		};

	public:
		template <TEventTypes NEvent>
		struct SingleHandler : public Handler {
			inline explicit SingleHandler(EventDispatch& event_dispatch) : Handler(event_dispatch, EventTypes(NEvent)) {}
			inline auto await_resume() // NOLINT(readability-identifier-naming)
				 -> decltype(auto)
			{
				if (!this->_event_dispatch._event_data)
					throw std::exception();
				return static_cast<typename TDataType<NEvent>::Type>(*this->_event_dispatch._event_data);
			}
		};
	};

}

