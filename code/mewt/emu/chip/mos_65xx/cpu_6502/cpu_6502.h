
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

		Address _pc = types::zero;
		Data _reg_a = types::zero;
		Data _reg_x = types::zero;
		Data _reg_y = types::zero;
		Data _reg_s = types::zero;
		flags_reg_t _reg_flags{ 0 };

		class InstructionUnit;

	public:
		cpu_6502_t(const clock_source_t& clock, MemoryInterface& memory_interface);
		async::Future<Data> read_data(Address address);
		async::Future<Address> read_address(Address address);
		async::Future<Address> read_address_zp(Data offset);
		async::Future<> write_data(Address address, Data data);
		async::Future<> run_inst();
		async::Future<> run_cpu();

		async::Future<> handle_branch(cpu_6502::Instruction::Branch::Op inst, Data imm_low);
		async::Future<> handle_call(cpu_6502::Instruction::Call::Op inst, Data imm_low, Data imm_high);
		async::Future<> handle_jump(cpu_6502::Instruction::Jump::Op inst, Address imm_addr);

		async::Future<> push(Data data);
		async::Future<Data> pop();
	};

}
