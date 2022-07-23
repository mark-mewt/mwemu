
#pragma once

#include "async/awaitable_func.decl.h"
#include "emu/clock.decl.h"

#include <stdint.h>

namespace mewt::emu::cpu
{

   class mos6502_t
   {
   private:
      const clock_source_t& _clock;
   public:
      mos6502_t(const clock_source_t& clock);
      async::awaitable_func_t<uint8_t> read_mem();
      async::awaitable_func_t<> write_mem(uint8_t v);
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_cpu();
   };

}

