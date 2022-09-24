
#pragma once

#include "mewt/async/event_dispatch.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/flags.h"
#include "mewt/types/intrusive_stack.impl.h"

#include <exception>

namespace mewt::async
{

	template <typename TEventData, typename TEventTypes, template <TEventTypes> class TDataType>
	inline void EventDispatch<TEventData, TEventTypes, TDataType>::dispatch(EventTypes event_types, const TEventData& event_data)
	{
		_event_data = &event_data;
		auto dispatch_stack = std::move(_handlers);
		while (!dispatch_stack.isEmpty())
		{
			auto& handler = *dispatch_stack.pop();
			if (handler._event_types.intersect(event_types).isEmpty())
				_handlers.push(handler);
			else
				handler._continuation();
		}
		_event_data = nullptr;
	}

	template <typename TEventData, typename TEventTypes, template <TEventTypes> class TDataType>
	class EventDispatch<TEventData, TEventTypes, TDataType>::Handler : public IntrusiveStack<Handler>::Node
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

	template <typename TEventData, typename TEventTypes, template <TEventTypes> class TDataType>
	template <TEventTypes NEvent>
	struct EventDispatch<TEventData, TEventTypes, TDataType>::SingleHandler : public Handler
	{
		using DataType = TDataType<NEvent>;
		inline explicit SingleHandler(EventDispatch& event_dispatch) : Handler(event_dispatch, EventTypes(NEvent)) {}
		inline auto await_resume() // NOLINT(readability-identifier-naming)
			 -> decltype(auto)
		{
			if (!this->_event_dispatch._event_data)
				throw std::exception();
			return static_cast<typename DataType::Type>(*this->_event_dispatch._event_data);
		}
	};

}
