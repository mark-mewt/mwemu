
#pragma once

#include "emu/clock.decl.h"
#include "types/intrusive_stack.h"
#include "types/coroutine.h"

namespace mewt::emu
{

   // todo: tidy this up!

   class clock_tick_awaiter_t;

   class clock_tick_count_t
   {
   public:
      inline clock_tick_count_t(const clock_source_t& clock, size_t tick_count)
         : _clock(clock), _tick_count(tick_count)
      {
      }
      inline clock_tick_awaiter_t operator co_await ();
   private:
      friend clock_tick_awaiter_t;
      friend clock_source_t;
      inline auto& clock() const { return _clock; }
      const clock_source_t& _clock;
      size_t _tick_count;
   };

   class clock_source_t
   {
   public:
      inline clock_tick_count_t next_tick() const { return { *this, 1 }; }
      void run();
   private:
      friend clock_tick_awaiter_t;
      //inline void add_awaiter(clock_tick_awaiter_t& awaiter) const { _awaiters.push(awaiter); }
      mutable mewt::intrusive_stack<clock_tick_awaiter_t> _awaiters;
   };

   class clock_tick_awaiter_t : public intrusive_stack<clock_tick_awaiter_t>::node_t
   {
   public:
      inline clock_tick_awaiter_t(clock_tick_count_t tick_count)
         : _tick_count(tick_count)
      {
      }
      inline bool await_ready() { return false; }
      inline void await_resume() { _continuation = {}; }
      inline bool await_suspend(std::coroutine_handle<> continuation)
      {
         _continuation = continuation;
         _tick_count._clock._awaiters.push(*this);
         return true;
      }
      //private: todo: make this whole class a private inner class, and eliminate any reason for the following to be private
      std::coroutine_handle<void> _continuation;
      clock_tick_count_t _tick_count;
   };

   inline clock_tick_awaiter_t clock_tick_count_t::operator co_await () { return *this; }

}
