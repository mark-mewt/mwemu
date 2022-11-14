
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/types/flags.h"
#include "mewt/emu/mem/interface/interface.Abstract.h"

#include <stdint.h>

namespace mewt::emu::chip::mos_65xx
{


	//class cpu_6502_t;

	//constexpr auto getBusSpecForDevice(types::traits::TypeId<cpu_6502_t>)
	//{
	//	return kCPUBusSpec;
	//}

	class cpu_6502_t
	{
	private:
		const clock_source_t& _clock;
		static constexpr mem::BusSpec<cpu_6502_t> kCPUBusSpec { .address_bits = { 16 } };
		//using AddressSpace = mem::AddressSpace<kCPUBusSpec>;
		using MemoryInterface = mem::interface::Abstract<kCPUBusSpec>;
		using AddressSpace = typename MemoryInterface::AddressSpace;
		MemoryInterface& _memory_interface;

	public:
		using Address = typename AddressSpace::Address;

	private:
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

		Address _pc { 0 };
		types::native::Byte _reg_a { 0 };
		types::native::Byte _reg_x { 0 };
		types::native::Byte _reg_y { 0 };
		types::native::Byte _reg_s { 0 };
		flags_reg_t _reg_flags;

		class InstructionUnit;

		static constexpr auto makeAddress(types::native::Byte low_byte,
													 types::native::Byte high_byte);
		static constexpr auto toAddressOffset(types::native::Word word);

	public:
		cpu_6502_t(const clock_source_t& clock, MemoryInterface& memory_interface);
		async::Future<types::native::Byte> read_data(Address address);
		async::Future<Address> read_address(Address address);
		async::Future<Address> read_address_zp(types::native::Byte offset);
		async::Future<> write_data(Address address, types::native::Byte data);
		async::Future<> run_inst();
		async::Future<> run_cpu();

		async::Future<> handle_branch(cpu_6502::Instruction::Branch::Op inst,
												types::native::Byte imm_low);
		async::Future<> handle_call(cpu_6502::Instruction::Call::Op inst, types::native::Byte imm_low,
											 types::native::Byte imm_high);
		async::Future<> handle_jump(cpu_6502::Instruction::Jump::Op inst, Address imm_addr);

		async::Future<> push(types::native::Byte data);
		async::Future<types::native::Byte> pop();
	};

}
