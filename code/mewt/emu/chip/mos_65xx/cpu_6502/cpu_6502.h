
#pragma once

#include "mewt/async/future_promise.decl.h"
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

      enum class flag_t : std::uint8_t
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
      using flags_reg_t = types::flags<flag_t>;

      address_t _pc = 0;
      data_t _reg_a = 0;
      data_t _reg_x = 0;
      data_t _reg_y = 0;
      data_t _reg_s = 0;
      flags_reg_t _reg_flags{ 0 };

   public:
      cpu_6502_t(const clock_source_t& clock, memory_interface_t& memory_interface);
      async::future<data_t> read_data(address_t address);
      async::future<address_t> read_address(address_t address);
      async::future<address_t> read_address_zp(data_t offset);
      async::future<> write_data(address_t address, data_t v);
      async::future<> run_inst();
      async::future<> run_cpu();

      async::future<> handle_branch(cpu_6502::branch_instruction_t inst, data_t immLow);
		async::future<> handle_call(cpu_6502::call_instruction_t inst, data_t immLow, data_t immHigh);
		async::future<> handle_jump(cpu_6502::jump_instruction_t inst, address_t immPtr);

      async::future<> push(data_t data);
      async::future<data_t> pop();

   };

}

