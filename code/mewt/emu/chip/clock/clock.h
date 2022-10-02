
#pragma once

#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/intrusive_stack.impl.h" // mwToDo: Fix!

namespace mewt::emu::chip
{

	// mwToDo: tidy this up!

	class clock_tick_awaiter_t;

	class ClockTickCount
	{
	public:
		inline ClockTickCount(const clock_source_t& clock, size_t tick_count)
			 : _clock(clock), _tick_count(tick_count)
		{
		}
		inline auto operator co_await() -> clock_tick_awaiter_t;

	private:
		friend clock_tick_awaiter_t;
		friend clock_source_t;
		[[nodiscard]] inline auto clock() const -> auto& { return _clock; }
		const clock_source_t& _clock;
		size_t _tick_count;
	};

	class clock_source_t
	{
	public:
		inline auto nextTick() const
			 -> ClockTickCount
		{
			return { *this, 1 };
		}
		void run();
		void tick();

	private:
		friend clock_tick_awaiter_t;
		// inline void add_awaiter(clock_tick_awaiter_t& awaiter) const { _awaiters.push(awaiter); }
		mutable mewt::IntrusiveStack<clock_tick_awaiter_t> _awaiters;
	};

	class clock_tick_awaiter_t : public IntrusiveStack<clock_tick_awaiter_t>::Node
	{
	public:
		inline explicit clock_tick_awaiter_t(ClockTickCount tick_count)
			 : _tick_count(tick_count)
		{
		}
		inline static auto await_ready() // NOLINT(readability-identifier-naming)
			 -> bool
		{
			return false;
		}
		inline void await_resume() // NOLINT(readability-identifier-naming)
		{
			_continuation = {};
		}
		inline auto await_suspend(std::coroutine_handle<> continuation) // NOLINT(readability-identifier-naming)
			 -> bool
		{
			_continuation = continuation;
			_tick_count._clock._awaiters.push(*this);
			return true;
		}

	private:
		// private: mwToDo: make this whole class a private inner class, and eliminate any reason for the following to be private
		std::coroutine_handle<void> _continuation;
		ClockTickCount _tick_count;
		friend clock_source_t;
	};

	inline auto ClockTickCount::operator co_await() -> clock_tick_awaiter_t { return clock_tick_awaiter_t(*this); }
}
