
#pragma once

#include "mewt/types/enum_traits.h"
#include "mewt/types/flags.decl.h"
#include "mewt/types/intrusive_stack.h"

namespace mewt::async
{

	template <typename TEventData, typename TEventTypes, template <TEventTypes> class TDataType>
	class EventDispatch {
	public:
		using EventTypes = types::flags<TEventTypes>;
		inline void dispatch(EventTypes event_types, const TEventData& event_data);

	private:
		const TEventData* _event_data = nullptr;
		class Handler;
		IntrusiveStack<Handler> _handlers;

	public:

		template <TEventTypes NEvent>
		struct SingleHandler;
	};

}

