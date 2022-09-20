
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
      MemoryInterface& _memory_interface;

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

      Address _pc = 0;
      Data _reg_a = 0;
      Data _reg_x = 0;
      Data _reg_y = 0;
      Data _reg_s = 0;
      flags_reg_t _reg_flags{ 0 };

		class InstructionUnit;

   public:
      cpu_6502_t(const clock_source_t& clock, MemoryInterface& memory_interface);
      async::future<Data> read_data(Address address);
      async::future<Address> read_address(Address address);
      async::future<Address> read_address_zp(Data offset);
      async::future<> write_data(Address address, Data data);
      async::future<> run_inst();
      async::future<> run_cpu();

      async::future<> handle_branch(cpu_6502::Instruction::Branch::Op inst, Data imm_low);
		async::future<> handle_call(cpu_6502::Instruction::Call::Op inst, Data imm_low, Data imm_high);
		async::future<> handle_jump(cpu_6502::Instruction::Jump::Op inst, Address imm_addr);

      async::future<> push(Data data);
      async::future<Data> pop();

   };

}

