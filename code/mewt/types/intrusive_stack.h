
#pragma once

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
		inline auto operator=(IntrusiveStack&& rhs) noexcept -> IntrusiveStack&;
		inline ~IntrusiveStack() = default;
		inline void push(TElt& elt) noexcept;
		[[nodiscard]] inline auto isEmpty() const noexcept -> bool { return _first == nullptr; }
		inline auto pop() noexcept -> TElt*;
   private:
      TElt* _first = nullptr;
      friend TElt;
		struct Node
		{
         TElt* _next = nullptr;
      };
   };

}

