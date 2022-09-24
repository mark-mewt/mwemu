
#pragma once

#include "mewt/types/intrusive_stack.h"

#include <utility>

namespace mewt
{

   template <typename TElt>
	inline auto IntrusiveStack<TElt>::operator = (IntrusiveStack && rhs) noexcept -> IntrusiveStack&
	{
		_first = rhs._first;
		rhs._first = nullptr;
	}
	template <typename TElt>
	inline void IntrusiveStack<TElt>::push(TElt& elt) noexcept
	{
		elt._next = _first;
		_first = std::addressof(elt);
	}
	template <typename TElt>
	inline auto IntrusiveStack<TElt>::pop() noexcept
		 -> TElt*
	{
		auto* elt = _first;
		if (_first)
			_first = _first->_next;
		return elt;
	}

}
