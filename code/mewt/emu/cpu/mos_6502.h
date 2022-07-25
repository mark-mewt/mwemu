
#pragma once

#include "async/awaitable_func.decl.h"
#include "emu/clock.decl.h"
#include "emu/memory_interface.h"

#include <stdint.h>

namespace mewt::emu::cpu
{

   class mos6502_t
   {
   private:
      using bus_spec_t = bus_spec<16, 8>;
      using address_t = bus_spec_t::address_t;
      using data_t = bus_spec_t::data_t;
      const clock_source_t& _clock;
      memory_interface<bus_spec_t>& _memory_interface;

      address_t _pc;

   public:
      mos6502_t(const clock_source_t& clock, memory_interface<bus_spec_t>& memory_interface);
      async::awaitable_func_t<data_t> read_data(address_t address);
      async::awaitable_func_t<address_t> read_address(address_t address);
      async::awaitable_func_t<> write_mem(data_t v);
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_cpu();
   };

}

