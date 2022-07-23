
#pragma once

#include "async/awaitable_func.decl.h"
#include "async/func_awaiter.decl.h"
#include "async/promise.h"

namespace mewt::async
{

   template<typename _ReturnType>
   class [[nodiscard]] awaitable_func_t
   {
   public:
      using promise_type = promise_t<_ReturnType>;
      inline func_awaiter_t<_ReturnType> operator co_await () { return *this; }
      inline awaitable_func_t(std::coroutine_handle<promise_type> handle) : _promise(handle.promise()) { }
      inline auto& promise() const { return _promise; }
   private:
      promise_type& _promise;
   };

}
