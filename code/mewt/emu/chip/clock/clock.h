
#pragma once

#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/types/coroutine.h"
#include "mewt/types/intrusive_stack.h"

namespace mewt::emu::chip
{

   // mwToDo: tidy this up!

   class clock_tick_awaiter_t;

	class ClockTickCountT
	{
   public:
		inline ClockTickCountT(const clock_source_t& clock, size_t tick_count)
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
		inline ClockTickCountT nextTick() const { return { *this, 1 }; }
		void run();
		void tick();
   private:
      friend clock_tick_awaiter_t;
      //inline void add_awaiter(clock_tick_awaiter_t& awaiter) const { _awaiters.push(awaiter); }
      mutable mewt::IntrusiveStack<clock_tick_awaiter_t> _awaiters;
   };

	class clock_tick_awaiter_t : public IntrusiveStack<clock_tick_awaiter_t>::Node
	{
   public:
		inline clock_tick_awaiter_t(ClockTickCountT tick_count)
			 : _tick_count(tick_count)
		{
      }
		inline auto await_ready() -> bool { return false; }
		inline void await_resume() { _continuation = {}; }
		inline auto await_suspend(std::coroutine_handle<> continuation) -> bool
		{
         _continuation = continuation;
         _tick_count._clock._awaiters.push(*this);
         return true;
      }
      //private: mwToDo: make this whole class a private inner class, and eliminate any reason for the following to be private
      std::coroutine_handle<void> _continuation;
		ClockTickCountT _tick_count;
	};

	inline auto ClockTickCountT::operator co_await() -> clock_tick_awaiter_t { return *this; }
}
