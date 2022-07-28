
#pragma once

#include "mewt/async/awaitable_func.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"
#include "mewt/types/flags.h"

#include <stdint.h>

namespace mewt::emu::chip::mos_65xx
{

   class cpu_6502_t
   {
   private:

      const clock_source_t& _clock;
      memory_interface_t& _memory_interface;

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
      cpu_6502_t(const clock_source_t& clock, memory_interface_t& memory_interface);
      async::awaitable_func_t<data_t> read_data(address_t address);
      async::awaitable_func_t<address_t> read_address(address_t address);
      async::awaitable_func_t<address_t> read_address_zp(data_t offset);
      async::awaitable_func_t<> write_data(address_t address, data_t v);
      async::awaitable_func_t<> run_inst();
      async::awaitable_func_t<> run_cpu();

      async::awaitable_func_t<> handle_branch(cpu_6502::branch_instruction_t inst, data_t immLow);
		async::awaitable_func_t<> handle_call(cpu_6502::call_instruction_t inst, data_t immLow, data_t immHigh);
		async::awaitable_func_t<> handle_jump(cpu_6502::jump_instruction_t inst, address_t immPtr);

      async::awaitable_func_t<> push(data_t data);
      async::awaitable_func_t<data_t> pop();

   };

}

