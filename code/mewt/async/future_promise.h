
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/types/non_movable.h"
#include "mewt/types/coroutine.h"

#include <variant>

namespace mewt::async {

	template <typename _ReturnType>
	struct future_base;

	template <typename _ReturnType>
	struct promise_base : types::non_movable_t {

		using future_type = future<_ReturnType>;
		using future_base_type = future_base<_ReturnType>;

		inline future_type get_return_object() noexcept { return *this; }
		inline void unhandled_exception() noexcept { _future->_value = std::current_exception(); }
		inline auto initial_suspend() noexcept { return std::suspend_never{}; }
		inline auto final_suspend() noexcept {
			if (_continuation)
				_continuation.resume();
			return std::suspend_never{};
		}
		inline ~promise_base() {
			if (_future)
				_future->_promise = nullptr;
		}

	protected:
		inline auto& future_value() { return _future->_value; }

	private:
		friend future_base_type;
		future_base_type* _future = nullptr;
		std::coroutine_handle<> _continuation;
	};

	struct void_type_t {};
	template <typename _ReturnType>
	using result_type = std::conditional_t<std::is_void_v<_ReturnType>, void_type_t, _ReturnType>;


	template <typename _ReturnType>
	struct future_base {

		using promise_type = promise<_ReturnType>;
		using promise_base_type = promise_base<_ReturnType>;

		inline future_base(promise_base_type& promise) noexcept : _promise(std::addressof(promise)) { _promise->_future = this; }
		inline future_base(future_base&& rhs) noexcept : _value(std::move(rhs._value)), _promise(rhs._promise) {
			rhs._promise = nullptr;
			if (_promise)
				_promise->_future = this;
		}

		inline future_base& operator=(future_base&& rhs) noexcept {
			_value = std::move(rhs._value);
			_promise = rhs._promise;
			rhs._promise = nullptr;
			if (_promise)
				_promise->_future = this;
		}

		inline ~future_base() noexcept {
			if (_promise)
				_promise->_future = nullptr;
		}

		inline bool await_ready() noexcept { return _promise == nullptr; }
		inline bool await_suspend(std::coroutine_handle<> continuation) noexcept {
			_promise->_continuation = continuation;
			return true;
		}

		future_base(const future_base&) = delete;
		future_base& operator=(const future_base&) = delete;

	protected:
		std::variant<std::monostate, result_type<_ReturnType>, std::exception_ptr> _value;

	private:
		friend promise_base_type;
		promise_base_type* _promise = nullptr;
	};

	template <typename _ReturnType>
	struct future : public future_base<_ReturnType> {
		using future_base<_ReturnType>::future_base;
		inline _ReturnType await_resume() { return std::get<_ReturnType>(this->_value); }
	};

	template <>
	struct future<void> : public future_base<void> {
		using future_base<void>::future_base;
		inline void await_resume() noexcept {}
	};

	template <typename _ReturnType>
	struct promise : public promise_base<_ReturnType> {
		inline void return_value(_ReturnType v) { this->future_value() = std::move(v); }
	};

	template <>
	struct promise<void> : promise_base<void> {
		inline void return_void() noexcept { this->future_value() = result_type<void>(); }
	};

}
