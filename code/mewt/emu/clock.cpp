
#include "emu/clock.h"

namespace mewt::emu
{

   void clock_source_t::run()
   {
      while (!_awaiters.is_empty())
      {
         auto prev_awaiters = std::move(_awaiters);
         while (auto* awaiter = prev_awaiters.pop())
         {
            if (--awaiter->_tick_count._tick_count == 0)
               awaiter->_continuation.resume();
            else
               _awaiters.push(*awaiter);
         }
      }
   }

}