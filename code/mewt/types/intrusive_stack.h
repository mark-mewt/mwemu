
#pragma once

#include <utility>

namespace mewt
{

   template<typename TElt>
   class IntrusiveStack
   {
   public:
		inline IntrusiveStack() = default;
		inline IntrusiveStack(const IntrusiveStack&) = delete;
		inline IntrusiveStack(IntrusiveStack&& rhs) noexcept : _first(rhs._first) { rhs._first = nullptr; }
		inline auto operator=(const IntrusiveStack&) -> IntrusiveStack& = delete;
		inline auto operator=(IntrusiveStack&& rhs) noexcept -> IntrusiveStack&
		{
			_first = rhs._first;
			rhs._first = nullptr;
		}
      inline void push(TElt& elt) noexcept
      {
         elt._next = _first;
         _first = std::addressof(elt);
      }
		[[nodiscard]] inline auto isEmpty() const noexcept -> bool { return _first == nullptr; }
		inline auto pop() noexcept
			 -> TElt*
      {
         auto* elt = _first;
         if (_first)
            _first = _first->_next;
         return elt;
      }
   private:
      TElt* _first = nullptr;
      friend TElt;
		struct Node
		{
         TElt* _next = nullptr;
      };
   };

}

