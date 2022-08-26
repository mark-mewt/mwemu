
#pragma once

#include "mewt/async/generator.decl.h"
#include "mewt/types/non_movable.h"
#include "mewt/types/coroutine.h"
#include "mewt/async/resumer.h"

namespace mewt::async {

	template <typename _ReturnType>
	struct generator {

		struct promise_type;
		using generator_coro_handle_t = std::coroutine_handle<promise_type>;

		struct promise_type : types::non_movable_t {

			inline generator<_ReturnType> get_return_object() noexcept { return generator_coro_handle_t::from_promise(*this); }

			inline void unhandled_exception() noexcept { _exception = std::current_exception(); }

			inline std::suspend_always initial_suspend() noexcept { return {}; }

			inline auto final_suspend() noexcept
			{
				if (_generator)
					_generator->_is_finished = true;
				return resumer_t(_continuation);
			}

			inline ~promise_type() { }

			// When the co-routine returns, store its return value in the future.
			inline void return_void() {
			}

			template<typename _From>
			auto yield_value(_From&& from) {
				_value = std::forward<_From>(from);
				return resumer_t(_continuation);
			}
	 
			// Pointer to the future. The future itself will update this on construction, destruction, and whenever it moves.
			generator* _generator = nullptr;

			// The continuation to allow us to resume the caller when we are done.
			std::coroutine_handle<> _continuation;

			_ReturnType _value;
			std::exception_ptr _exception;

		};

		inline generator(generator_coro_handle_t generator_coro) noexcept : _generator_coro(generator_coro) { promise()._generator = this; }

		// If the generator moves, we must inform the promise of our new location.
		inline generator(generator&& rhs) noexcept : _generator_coro(rhs._generator_coro) {
			rhs._generator_coro = {};
			if (!is_finished())
				promise()._generator = this;
		}

		// If the generator is destroyed before the promise, we must tell it.
		inline ~generator() noexcept {
			if (!is_finished())
				promise()._generator = nullptr;
			_generator_coro.destroy();
		}

		inline bool await_ready() noexcept { return is_finished(); }

		inline bool is_finished() const noexcept { return _is_finished; }

		// When we do suspend, we store the continuation in the promise so it can resume once the generator next yields.
		inline auto await_suspend(std::coroutine_handle<> continuation) noexcept {
			promise()._continuation = continuation;
			return _generator_coro;
		}

		// When the awaiter resumes, we return the value that the co-routine returned.
		// mwToDo: Re-throw exeption here if one was stored.
		inline _ReturnType& await_resume()
		{
			if (promise()._exception)
				std::rethrow_exception(promise()._exception);
			return promise()._value;
		}

		// We don't allow copying or assignment of the generator.
		generator(const generator&) = delete;
		generator& operator=(const generator&) = delete;
		generator& operator=(generator&&) = delete;

	private:
		// The promise we are linked to.
		generator_coro_handle_t _generator_coro;
		//promise_type* _promise = nullptr;
		inline auto& promise() { return _generator_coro.promise(); }
		bool _is_finished = false;
	};

}
