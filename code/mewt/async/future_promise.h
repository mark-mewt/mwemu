
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/async/resumer.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/non_movable.h"

#include <variant>

namespace mewt::async {

	struct VoidType {};

	// future_base determines the functionality of a future, regardless of the return type.
	// We'll specialize this later to handle the void return type.
	template <typename TReturnType>
	struct FutureBase {

		// Our promise promises to:
		// (a) Not change address - we do this by making it a non_movable_t.
		// (b) Notify the future on destruction (this combined with (a) prevents a dangling reference).
		// (c) Pass the return value of the co-routine back to the future.
		// (d) Capture any exception that is thrown and re-throw it in the frame of the caller.
		// (e) Resume the caller once this co-routine completes.
		class PromiseBase : types::non_movable_t
		{

		public:
			// The return type of the co-routine is the future. We initialize it with a reference to
			// this promise. The future will then tell us what it's address is, and will update this
			// whenever it moves. The future we return here won't actually be returned to the calling
			// function until after the co-routine suspends, which may not be until the co-routine
			// completes, which may be after this promise is destroyed.
			inline auto get_return_object() noexcept // NOLINT(readability-identifier-naming)
				 -> Future<TReturnType>
			{
				return Future<TReturnType>(*this);
			}

			// If an exception is thrown in the co-routine, we pass it on to the future to be re-thrown.
			inline void unhandled_exception() noexcept // NOLINT(readability-identifier-naming)
			{
				if (_future == nullptr)
					abort();
				_future->_value = std::current_exception();
			}

			// We want the co-routine to start immediately, so we won't suspend initially.
			inline auto initial_suspend() noexcept // NOLINT(readability-identifier-naming)
			{
				return std::suspend_never{};
			}

			// When the co-routine finishes, at the final suspend point we resume the continuation.
			inline auto final_suspend() noexcept // NOLINT(readability-identifier-naming)
			{
				auto continuation = _continuation;
				// auto hh = std::coroutine_handle<promise_base_t>::from_promise(*this);

				// hh.destroy();
				return Resumer(continuation);
			}

			// When the promise is destroyed, inform the future.
			inline ~PromiseBase()
			{
				if (_future)
					_future->_promise = nullptr;
			}

			// Reference the return value stored in the future.
			template <typename TValue>
			inline void setFutureValue(TValue&& value)
			{
				if (_future)
					_future->_value.template emplace<ResultType>(std::forward<TValue>(value));
			}

		//protected:
			[[nodiscard]] inline auto future() const { return _future; }
		//	inline void setFuture(FutureBase* future) { _future = future; }

		private:
			friend FutureBase;

			// Pointer to the future. The future itself will update this on construction, destruction, and whenever it moves.
			FutureBase* _future = nullptr;

			// The continuation to allow us to resume the caller when we are done.
			std::coroutine_handle<> _continuation;
		};

		// Every future is created by its promise. We must tell the promise where we are.
		inline explicit FutureBase(PromiseBase& promise) noexcept : _promise(std::addressof(promise)) { _promise->_future = this; }

		// If the future moves, we must inform the promise of our new location.
		inline FutureBase(FutureBase&& rhs) noexcept : _value(std::move(rhs._value)), _promise(rhs._promise) {
			rhs._promise = nullptr;
			if (_promise)
				_promise->_future = this;
		}

		// If the future is destroyed before the promise, we must tell it.
		inline ~FutureBase() noexcept {
			if (_promise)
				_promise->_future = nullptr;
		}

		// When the future is awaited, if the promise has already finished then we don't need to suspend.
		inline auto await_ready() noexcept // NOLINT(readability-identifier-naming)
			 -> bool
		{
			return _promise == nullptr;
		}

		// When we do suspend, we store the continuation in the promise so it can resume once the co-routine finishes.
		inline auto await_suspend(std::coroutine_handle<> continuation) noexcept // NOLINT(readability-identifier-naming)
			 -> bool
		{
			_promise->_continuation = continuation;
			return true;
		}

		// We don't allow copying or assignment of the future.
		FutureBase(const FutureBase&) = delete;
		auto operator=(const FutureBase&) = delete;
		auto operator=(FutureBase&&) = delete;

	protected:
		// We can't store void in a variant so we'll replace that with void_type_t.
		using ResultType = std::conditional_t<std::is_void_v<TReturnType>, VoidType, TReturnType>;

		[[nodiscard]] inline auto promise() const { return _promise; }	// mwToDo: Should be able to get rid of this.
		[[nodiscard]] inline auto value() const -> auto& { return _value; }
	private:
		// The value of the future is one of std::monostate (not yet completed), the result of the co-routine, or the exception it threw.
		std::variant<std::monostate, ResultType, std::exception_ptr> _value;

		// The promise we are linked to.
		PromiseBase* _promise = nullptr;
	};

	// Here we provide the future/promise functionality for a return type other than void.
	template <typename TReturnType>
	struct Future : public FutureBase<TReturnType> {

		// Defer construction to future_base.
		using FutureBase<TReturnType>::FutureBase;

		// When the awaiter resumes, we return the value that the co-routine returned.
		// mwToDo: Re-throw exeption here if one was stored.
		inline auto await_resume() // NOLINT(readability-identifier-naming)
			 -> TReturnType
		{
			// mwToDo: Tidy this up - there should be a better place to do this.
			if (this->promise())
				std::coroutine_handle<typename FutureBase<TReturnType>::PromiseBase>::from_promise(*this->promise()).destroy();
			return std::get<TReturnType>(this->value());
		}

		// The promise for a non-void return type.
		struct PromiseType : public FutureBase<TReturnType>::PromiseBase {

			// When the co-routine returns, store its return value in the future.
			template<typename TValue>
			inline void return_value(TValue&& value) // NOLINT(readability-identifier-naming)
			{
				this->setFutureValue(std::forward<TValue>(value));
			}
		};

		using promise_type = PromiseType;

	};

	// And this is the functionality specifically for a void return type.
	template <>
	struct Future<void> : public FutureBase<void> {

		// Defer construction to future_base.
		using FutureBase<void>::FutureBase;

		// Nothing is returned to the awaiter here.
		// mwToDo: Re-throw exeption here if one was stored.
		inline void await_resume() noexcept // NOLINT(readability-identifier-naming)
		{
			// mwToDo: Tidy this up - there should be a better place to do this.
			if (this->promise() != nullptr)
				std::coroutine_handle<PromiseBase>::from_promise(*this->promise()).destroy();
		}

		// The promise for a void return type.
		struct PromiseType : PromiseBase {

			// When the co-routine completes, switch the return value stored in the future to the void type.
			inline void return_void() noexcept // NOLINT(readability-identifier-naming)
			{
				if(this->future() != nullptr)
					this->setFutureValue(VoidType()); 
			}
		};
		using promise_type = PromiseType;
	};

}
