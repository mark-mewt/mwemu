
#pragma once

#include "async/promise_value.h"

#include <coroutine>

namespace mewt::async
{

   template<typename _ReturnType>
   class promise_t : public promise_value_t<_ReturnType>
   {
   public:
      auto initial_suspend() { return std::suspend_never{ }; }
      auto final_suspend() noexcept
      {
         // todo: because of noexcept, we really want to return an awaitable, which resumes the continuation for us when that is awaited
         if (_continuation)
            _continuation.resume();
         return std::suspend_never{};
      }
      auto get_return_object() { return std::coroutine_handle<promise_t>::from_promise(*this); }
      void unhandled_exception() { /* do something here */ }
      void set_continuation(std::coroutine_handle<> continuation) { _continuation = continuation; }
   private:
      std::coroutine_handle<> _continuation;
   };

}
