
#pragma once

#include "async/awaitable_func.decl.h"
#include "emu/clock.decl.h"

namespace mewt::emu::gpu
{

   class vic2_t
   {
   private:
      const clock_source_t& _clock;
   public:
      vic2_t(const clock_source_t& clock);
      async::awaitable_func_t<> read_mem();
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_gpu();
   };

}
