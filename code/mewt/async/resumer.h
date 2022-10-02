
#pragma once

#include "mewt/types/coroutine.h"

namespace mewt::async
{

	class Resumer
	{
	public:
		explicit Resumer(std::coroutine_handle<> coro) noexcept : _coro(coro) {}
		inline auto await_ready() noexcept // NOLINT(readability-identifier-naming)
			 -> bool
		{
			return _coro == nullptr;
		}
		inline auto await_suspend(std::coroutine_handle<> /*continuation*/) noexcept // NOLINT(readability-identifier-naming)
		{
			return _coro;
		}
		inline void await_resume() noexcept {} // NOLINT(readability-identifier-naming)

	private:
		std::coroutine_handle<> _coro;
	};

}
