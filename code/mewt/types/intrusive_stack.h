
#pragma once

#include <utility>

namespace mewt
{

   template<typename _Elt>
   class intrusive_stack
   {
   public:
      inline intrusive_stack() = default;
      inline intrusive_stack(const intrusive_stack&) = delete;
      inline intrusive_stack(intrusive_stack&& rhs) noexcept : _first(rhs._first) { rhs._first = nullptr; }
      inline intrusive_stack& operator = (const intrusive_stack&) = delete;
      inline intrusive_stack& operator = (intrusive_stack&& rhs) noexcept { _first = rhs._first; rhs._first = nullptr; }
      inline void push(_Elt& elt) noexcept
      {
         elt._next = _first;
         _first = std::addressof(elt);
      }
      inline bool is_empty() const noexcept { return _first == nullptr; }
      inline _Elt* pop() noexcept
      {
         auto* elt = _first;
         if (_first)
            _first = _first->_next;
         return elt;
      }
   private:
      _Elt* _first = nullptr;
      friend _Elt;
      struct node_t
      {
         _Elt* _next = nullptr;
      };
   };

}

