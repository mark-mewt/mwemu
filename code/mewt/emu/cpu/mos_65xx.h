
#pragma once

#include "async/awaitable_func.decl.h"
#include "emu/clock.decl.h"
#include "emu/memory_interface.h"
#include "emu/cpu/mos_65xx/mos65xx_instructions.h"
#include "types/flags.h"

#include <stdint.h>

namespace mewt::emu::cpu
{

   class mos65xx_t
   {
   private:
      using bus_spec_t = bus_spec<16, 8>;
      using address_t = bus_spec_t::address_t;
      using data_t = bus_spec_t::data_t;
      const clock_source_t& _clock;
      memory_interface<bus_spec_t>& _memory_interface;

      enum class flag_t
      {
         Carry,
         Zero,
         InterruptEnable,
         Decimal,
         Break,
         _,
         Overflow,
         Negative
      };
      using flags_reg_t = types::flags<flag_t, 8>;

      address_t _pc;
      data_t _reg_a;
      data_t _reg_x;
      data_t _reg_y;
      data_t _reg_s;
      flags_reg_t _reg_flags{ 0 };

   public:
      mos65xx_t(const clock_source_t& clock, memory_interface<bus_spec_t>& memory_interface);
      async::awaitable_func_t<data_t> read_data(address_t address);
      async::awaitable_func_t<address_t> read_address(address_t address);
      async::awaitable_func_t<address_t> read_address_zp(data_t offset);
      async::awaitable_func_t<> write_data(address_t address, data_t v);
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_cpu();

      async::awaitable_func_t<> handle_branch(mos65xx::branch_instruction_t inst, data_t immLow);
      async::awaitable_func_t<> handle_call(mos65xx::call_instruction_t inst, data_t immLow, data_t immHigh);
      async::awaitable_func_t<> handle_jump(mos65xx::jump_instruction_t inst, address_t immPtr);

      async::awaitable_func_t<> push(data_t data);
      async::awaitable_func_t<data_t> pop();

   };

}

