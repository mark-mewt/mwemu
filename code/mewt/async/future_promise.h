
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/async/resumer.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/non_movable.h"

#include <variant>

namespace mewt::async {

	struct void_type_t {};

	// future_base determines the functionality of a future, regardless of the return type.
	// We'll specialize this later to handle the void return type.
	template <typename _ReturnType>
	struct future_base {

		// Our promise promises to:
		// (a) Not change address - we do this by making it a non_movable_t.
		// (b) Notify the future on destruction (this combined with (a) prevents a dangling reference).
		// (c) Pass the return value of the co-routine back to the future.
		// (d) Capture any exception that is thrown and re-throw it in the frame of the caller.
		// (e) Resume the caller once this co-routine completes.
		struct promise_base_t : types::non_movable_t {

			// The return type of the co-routine is the future. We initialize it with a reference to
			// this promise. The future will then tell us what it's address is, and will update this
			// whenever it moves. The future we return here won't actually be returned to the calling
			// function until after the co-routine suspends, which may not be until the co-routine
			// completes, which may be after this promise is destroyed.
			inline future<_ReturnType> get_return_object() noexcept { return *this; }

			// If an exception is thrown in the co-routine, we pass it on to the future to be re-thrown.
			inline void unhandled_exception() noexcept {
				if (_future == nullptr)
					abort();
				_future->_value = std::current_exception();
			}

			// We want the co-routine to start immediately, so we won't suspend initially.
			inline auto initial_suspend() noexcept { return std::suspend_never{}; }

			// When the co-routine finishes, at the final suspend point we resume the continuation.
			inline auto final_suspend() noexcept {
				auto continuation = _continuation;
				//auto hh = std::coroutine_handle<promise_base_t>::from_promise(*this);
				
				//hh.destroy();
				return resumer_t(continuation);
			}

			// When the promise is destroyed, inform the future.
			inline ~promise_base_t() {
				if (_future)
					_future->_promise = nullptr;
			}

			// Reference the return value stored in the future.
			template<typename _Value>
			inline void set_future_value(_Value&& value) {
				if (_future)
					_future->_value.template emplace<result_type>(std::forward<_Value>(value));
			}

			// Pointer to the future. The future itself will update this on construction, destruction, and whenever it moves.
			future_base* _future = nullptr;

			// The continuation to allow us to resume the caller when we are done.
			std::coroutine_handle<> _continuation;
		};

		// Every future is created by its promise. We must tell the promise where we are.
		inline future_base(promise_base_t& promise) noexcept : _promise(std::addressof(promise)) { _promise->_future = this; }

		// If the future moves, we must inform the promise of our new location.
		inline future_base(future_base&& rhs) noexcept : _value(std::move(rhs._value)), _promise(rhs._promise) {
			rhs._promise = nullptr;
			if (_promise)
				_promise->_future = this;
		}

		// If the future is destroyed before the promise, we must tell it.
		inline ~future_base() noexcept {
			if (_promise)
				_promise->_future = nullptr;
		}

		// When the future is awaited, if the promise has already finished then we don't need to suspend.
		inline bool await_ready() noexcept { return _promise == nullptr; }

		// When we do suspend, we store the continuation in the promise so it can resume once the co-routine finishes.
		inline bool await_suspend(std::coroutine_handle<> continuation) noexcept {
			_promise->_continuation = continuation;
			return true;
		}

		// We don't allow copying or assignment of the future.
		future_base(const future_base&) = delete;
		future_base& operator=(const future_base&) = delete;
		future_base& operator=(future_base&&) = delete;

	protected:
		// We can't store void in a variant so we'll replace that with void_type_t.
		using result_type = std::conditional_t<std::is_void_v<_ReturnType>, void_type_t, _ReturnType>;

		// The value of the future is one of std::monostate (not yet completed), the result of the co-routine, or the exception it threw.
		std::variant<std::monostate, result_type, std::exception_ptr> _value;

		inline auto* promise() const { return _promise; }	// mwToDo: Should be able to get rid of this.
	private:
		// The promise we are linked to.
		promise_base_t* _promise = nullptr;
	};

	// Here we provide the future/promise functionality for a return type other than void.
	template <typename _ReturnType>
	struct future : public future_base<_ReturnType> {

		// Defer construction to future_base.
		using future_base<_ReturnType>::future_base;

		// When the awaiter resumes, we return the value that the co-routine returned.
		// mwToDo: Re-throw exeption here if one was stored.
		inline _ReturnType await_resume()
		{
			// mwToDo: Tidy this up - there should be a better place to do this.
			if (this->promise())
				std::coroutine_handle<typename future_base<_ReturnType>::promise_base_t>::from_promise(*this->promise()).destroy();
			return std::get<_ReturnType>(this->_value);
		}

		// The promise for a non-void return type.
		struct promise_type : public future_base<_ReturnType>::promise_base_t {

			// When the co-routine returns, store its return value in the future.
			template<typename _Value>
			inline void return_value(_Value&& value) { this->set_future_value(std::forward<_Value>(value)); }
		};
	};

	// And this is the functionality specifically for a void return type.
	template <>
	struct future<void> : public future_base<void> {

		// Defer construction to future_base.
		using future_base<void>::future_base;

		// Nothing is returned to the awaiter here.
		// mwToDo: Re-throw exeption here if one was stored.
		inline void await_resume() noexcept
		{
			// mwToDo: Tidy this up - there should be a better place to do this.
			if (this->promise())
				std::coroutine_handle<promise_base_t>::from_promise(*this->promise()).destroy();
		}

		// The promise for a void return type.
		struct promise_type : promise_base_t {

			// When the co-routine completes, switch the return value stored in the future to the void type.
			inline void return_void() noexcept { if(this->_future) this->set_future_value(void_type_t()); }
		};
	};

}
