
#pragma once

namespace mewt::async
{

   template<typename _ReturnType>
   struct func_awaiter_base_t
   {
      func_awaiter_base_t(awaitable_func_t<_ReturnType>& func)
         : _func(func)
      {
      }
      constexpr bool await_ready() { return false; }
      bool await_suspend(std::coroutine_handle<> continuation);
   protected:
      awaitable_func_t<_ReturnType>& _func;
   };

   template<typename _ReturnType>
   struct func_awaiter_t : public func_awaiter_base_t<_ReturnType>
   {
      using func_awaiter_base_t<_ReturnType>::func_awaiter_base_t;
      _ReturnType await_resume() { return *this->_func.promise()._return_value; }
   };

   template<> // todo: we can merge this back into above using if constexpr
   struct func_awaiter_t<void> : public func_awaiter_base_t<void>
   {
      using func_awaiter_base_t<void>::func_awaiter_base_t;
      void await_resume() { }
   };

   template<typename _ReturnType>
   bool func_awaiter_base_t<_ReturnType>::await_suspend(std::coroutine_handle<> continuation)
   {
      _func.promise().set_continuation(continuation);
      //_continuation = continuation;
      // when _func is done, we need to resume _continuation
      // how do we know when _func is done?
      return true;
   }

}
