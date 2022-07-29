
#pragma once

#include "mewt/types/non_movable.h"
#include "mewt/async/future.h"

#include <coroutine>

namespace mewt::async
{

   struct resumer_t
   {
      std::coroutine_handle<> _continuation;
      constexpr bool await_ready() const noexcept
      {
         return !_continuation;
      }

      constexpr bool await_suspend(std::coroutine_handle<> h) const noexcept
      {
			//return ((bool)_continuation) ? _continuation : h;
         if (_continuation)
            _continuation.resume();
			return false;
      }
      constexpr void await_resume() const noexcept
      {
      }
   };

   template <typename _ReturnType>
	struct promise_value_base_t : non_movable_t {
		future<_ReturnType>::reference_t _future;
	};

	template <typename _ReturnType>
	struct promise_value_t : public promise_value_base_t<_ReturnType> {
		inline void return_value(_ReturnType v) { this->_future->_return_value.emplace(std::move(v)); }
	};

	template <>
	struct promise_value_t<void> : promise_value_base_t<void> {
		inline void return_void() { this->_future->_is_complete = true; }
	};

	template <typename _ReturnType>
   class promise_t : public promise_value_t<_ReturnType>
   {
   public:
      auto initial_suspend() { return std::suspend_never{ }; }
      auto final_suspend() noexcept
      {
         //return resumer_t{this->_future->_continuation};
         // todo: because of noexcept, we really want to return an awaitable, which resumes the continuation for us when that is awaited
         if (this->_future->_continuation)
            this->_future->_continuation.resume();
         return std::suspend_never{};
      }
      auto get_return_object() 
      {
         auto [future, ref] = types::make_unique_reference<future<_ReturnType>>();
         this->_future = std::move(ref);
         return std::move(future);
         //return !this->_is_complete ? std::coroutine_handle<promise_t>::from_promise(*this) : std::coroutine_handle<promise_t>{};
      }
      inline ~promise_t()
      {
      }
      void unhandled_exception() {
			/* do something here */
			auto ex = std::current_exception();
			auto x1 = std::coroutine_handle<promise_t>::from_promise(*this);
			auto x2 = std::coroutine_handle<promise_t>::from_promise(*this);
			// todo: store ex in variant of future, then std::rethrow_exception when the caller is resumed.
		}
      //void set_continuation(std::coroutine_handle<> continuation) { _continuation = continuation; }
   private:
   };

}
