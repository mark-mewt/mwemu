
#pragma once

#include "mewt/types/coroutine.h"

namespace mewt::async {

	class resumer_t {
	public:
		resumer_t(std::coroutine_handle<> coro) noexcept : _coro(coro) {}
		inline bool await_ready() noexcept { return _coro == nullptr; }
		inline auto await_suspend(std::coroutine_handle<> continuation) noexcept { return _coro; }
		inline void await_resume() noexcept {}

	private:
		std::coroutine_handle<> _coro;
	};

}
