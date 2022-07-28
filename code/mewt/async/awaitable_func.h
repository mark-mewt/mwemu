
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
      inline awaitable_func_t(async::future<_ReturnType>&& future)
         : _future(std::move(future))
      {
         //if (handle.done())
         // /  this->set_complete();
         //else 
         //   _promise._return_value = this;
      }
      //inline auto& promise() const { return _promise; }
      inline auto& future() { return _future; }
   private:
      async::future<_ReturnType> _future;
   };

}


// for a coroutine that takes a path that doesn't suspend, and just returns, it appears the promise is destroyed before the return type is created.
