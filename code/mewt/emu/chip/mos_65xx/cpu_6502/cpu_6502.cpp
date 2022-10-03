
#include "mewt/async/future_promise.h"
#include "mewt/diag/log.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502.h"
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"
#include "mewt/types/intrusive_stack.h"
#include "mewt/types/byte.h"

/*
 *
 * mwToDo: Write a splitter for clock cycles, so we can run things on sub-cycles.
 * mwToDo: Can we separate the list of awaiters from the clock source (and remove the mutable)?
 * mwToDo: Get C64 rom and start emulating it
 * mwToDo: Start emulating the Vic-2
 * mwToDo: Enable all warnings in compiler - need to shift out the win32 stuff first
 * mwToDo: Make address sanitizer work
 *
 */

namespace mewt::emu::chip::mos_65xx
{

	constexpr Data kDecimalMax = types::fromUnderlyingType<Data>(99);

	cpu_6502_t::cpu_6502_t(const clock_source_t& clock, MemoryInterface& memory_interface)
		 : _clock(clock), _memory_interface(memory_interface)
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
	}

	auto cpu_6502_t::read_data(Address address)
		 -> async::Future<Data>
	{
		co_await _clock.nextTick();
		auto data = _memory_interface.read(address);
		// logger().log("[0x%04X] -> 0x%02X", address, data);
		co_return data;
	}

	auto cpu_6502_t::read_address(Address address)
		 -> async::Future<Address>
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		types::Byte low = co_await read_data(address);
		types::Byte high = co_await read_data(++address);
		co_return types::makeWord(low, high);
	}

	auto cpu_6502_t::read_address_zp(Data offset)
		 -> async::Future<Address>
	{
		return read_address(types::makeWord(offset, types::zero));
	}

	auto cpu_6502_t::write_data(Address address, Data data)
		 -> async::Future<>
	{
		// logger().log("[0x%04X] <- 0x%02X", address, data);
		co_await _clock.nextTick();
		co_return _memory_interface.write(address, data);
	}

	class cpu_6502_t::InstructionUnit
	{
	public:
		explicit InstructionUnit(cpu_6502_t& cpu) : _cpu(cpu) {}
		auto fetch() -> async::Future<>;
		auto handleBranch() -> async::Future<bool>;
		auto loadSource() -> async::Future<Data>;
		auto loadReference() -> Data;
		void execute(Data& val, Data ref);
		void processFlags(bool carry_flag, bool overflow_flag, Data val);
		auto storeResult(Data val) -> async::Future<>;

	private:
		cpu_6502_t& _cpu;
		const cpu_6502::Instruction* _inst = nullptr;
		Data _inst_code = types::zero;
		Data _imm_high = types::zero;
		Data _imm_low = types::zero;
		Address _imm_addr = types::zero;
	};

	auto cpu_6502_t::InstructionUnit::fetch()
		 -> async::Future<>
	{
		_inst_code = co_await _cpu.read_data(_cpu._pc);
		_inst = std::addressof(cpu_6502::getInstructions()[asUnderlyingType(_inst_code)]);
		++_cpu._pc;
		_imm_low = co_await _cpu.read_data(_cpu._pc);
		if (_inst->is_3_byte())
		{
			++_cpu._pc;
			_imm_high = co_await _cpu.read_data(_cpu._pc);
			_imm_addr = types::makeWord(_imm_low, _imm_high);
			// logger().log("0x%04X: %02X %s(%04X)", _pc, instCode, to_string(inst.opcode), immAddr);
		}
		else
		{
			// logger().log("0x%04X: %02X %s(%02X)", _pc, instCode, to_string(inst.opcode), immLow);
		}
		if (!_inst->is_1_byte())
			++_cpu._pc;
	}

	auto cpu_6502_t::InstructionUnit::handleBranch()
		 -> async::Future<bool>
	{
		co_await std::suspend_never(); // mwToDo: Get rid, fix clang static analysis errors
		if (_inst->is_branch())
		{
			co_await _cpu.handle_branch(cpu_6502::Instruction::Branch::fromInstruction(asUnderlyingType(_inst_code)), _imm_low);
			co_return true;
		}
		if (_inst->is_call())
		{
			co_await _cpu.handle_call(cpu_6502::Instruction::Call::fromInstruction(asUnderlyingType(_inst_code)), _imm_low, _imm_high);
			co_return true;
		}
		if (_inst->is_jump())
		{
			co_await _cpu.handle_jump(cpu_6502::Instruction::Jump::fromInstruction(asUnderlyingType(_inst_code)), _imm_addr);
			co_return true;
		}
		co_return false;
	}

	auto cpu_6502_t::InstructionUnit::loadSource()
		 -> async::Future<Data>
	{
		co_await std::suspend_never(); // mwToDo: Get rid, fix clang static analysis errors
		using cpu_6502::data_loc_t;
		switch (_inst->src)
		{
		case data_loc_t::Imm8:
			co_return _imm_low;
		case data_loc_t::Flag:
			co_return types::fromUnderlyingType<Data>(_cpu._reg_flags.rawBits());
		case data_loc_t::RegX:
			co_return _cpu._reg_x;
		case data_loc_t::RegY:
			co_return _cpu._reg_y;
		case data_loc_t::RegA:
			co_return _cpu._reg_a;
		case data_loc_t::RegS:
			co_return _cpu._reg_s;
		case data_loc_t::PtrX:
			co_return co_await _cpu.read_data(_imm_addr + _cpu._reg_x);
		case data_loc_t::PtrY:
			co_return co_await _cpu.read_data(_imm_addr + _cpu._reg_y);
		case data_loc_t::Ptr_:
			co_return co_await _cpu.read_data(_imm_addr);
		case data_loc_t::Zpge:
			co_return co_await _cpu.read_data(types::makeWord(_imm_low, types::zero));
		case data_loc_t::ZppX:
			co_return co_await _cpu.read_data(types::makeWord(_imm_low + _cpu._reg_x, types::zero));
		case data_loc_t::ZppY:
			co_return co_await _cpu.read_data(types::makeWord(_imm_low + _cpu._reg_y, types::zero));
		case data_loc_t::IndY:
			co_return co_await _cpu.read_data(co_await _cpu.read_address_zp(_imm_low) + _cpu._reg_y);
		case data_loc_t::Stck:
			co_return co_await _cpu.pop();
		default:
			throw std::exception("implement");
		}
	}

	const Data kBit0 = types::fromUnderlyingType<Data>(1 << 0);
	const Data kBit1 = types::fromUnderlyingType<Data>(1 << 1);
	const Data kBit2 = types::fromUnderlyingType<Data>(1 << 2);
	const Data kBit3 = types::fromUnderlyingType<Data>(1 << 3);
	// const Data kBit4 = (1 << 4);
	// const Data kBit5 = (1 << 5);
	const Data kBit6 = types::fromUnderlyingType<Data>(1 << 6);
	const Data kBit7 = types::fromUnderlyingType<Data>(1 << 7);

	auto cpu_6502_t::InstructionUnit::loadReference()
		 -> Data
	{
		using cpu_6502::data_loc_t;
		switch (_inst->ref)
		{
		case data_loc_t::None:
			return types::zero;
		case data_loc_t::Flag:
			return types::fromUnderlyingType<Data>(_cpu._reg_flags.rawBits());
		case data_loc_t::RegA:
			return _cpu._reg_a;
		case data_loc_t::RegX:
			return _cpu._reg_x;
		case data_loc_t::RegY:
			return _cpu._reg_y;
		case data_loc_t::Bit0:
			return kBit0;
		case data_loc_t::Bit1:
			return kBit1;
		case data_loc_t::Bit2:
			return kBit2;
		case data_loc_t::Bit3:
			return kBit3;
		default:
			throw std::exception("implement");
		}
	}

	void cpu_6502_t::InstructionUnit::execute(Data& val, Data ref)
	{
		using cpu_6502::operation_t;
		using cpu_6502::data_loc_t;
		bool carry_flag = _cpu._reg_flags[flag_t::Carry];
		bool overflow_flag = _cpu._reg_flags[flag_t::Overflow];
		switch (_inst->oper)
		{
		case operation_t::Copy:
			break;
		case operation_t::Or__:
			val |= ref;
			break;
		case operation_t::AndN:
			val &= ~ref;
			break;
		case operation_t::And_:
			val &= ref;
			break;
		case operation_t::Sub_: {
			const bool signs_equal = (val & kBit7) == (ref & kBit7);
			val = ref - val;
			if (_inst->dest != data_loc_t::None)
			{
				val -= types::fromUnderlyingType<Data>(carry_flag ? 0 : 1);
				const bool signs_changed = (val & kBit7) == (ref & kBit7);
				overflow_flag = signs_equal && signs_changed;
			}
			carry_flag = (val & kBit7) == types::zero;
		}
		break;
		case operation_t::Add_: {
			const bool signs_equal = (val & kBit7) == (ref & kBit7);
			const auto val16 = static_cast<uint16_t>(ref) + static_cast<uint16_t>(val);
			val = static_cast<Data>(val16);
			if (_inst->dest != data_loc_t::None)
			{
				val += types::fromUnderlyingType<Data>(carry_flag ? 1 : 0);
				const bool signs_changed = (val & kBit7) == (ref & kBit7);
				overflow_flag = signs_equal && signs_changed;
			}
			if (_cpu._reg_flags[flag_t::Decimal])
				carry_flag = (val16 > asUnderlyingType(kDecimalMax));
			else
				carry_flag = types::highByte(val16) != 0;
		}
		break;
		case operation_t::Dec_:
			val -= ref;
			break;
		case operation_t::Inc_:
			val += ref;
			break;
		case operation_t::Rol_: {
			auto old_carry = carry_flag;
			carry_flag = (val & kBit7) != types::zero;
			val = (val << 1) | types::fromUnderlyingType<Data>(old_carry ? 1 : 0);
		}
		break;
		case operation_t::Asl_: {
			carry_flag = (val & kBit7) != types::zero;
			val = (val << 1);
		}
		break;
		case operation_t::Lsr_: {
			carry_flag = (val & kBit0) != types::zero;
			val = (val >> 1);
		}
		break;
		case operation_t::Xor_:
			val ^= ref;
			break;
		case operation_t::BTst: {
			_cpu._reg_flags[flag_t::Negative] = (val & kBit7) != types::zero; // set negative flag if value is negative
			_cpu._reg_flags[flag_t::Overflow] = (val & kBit6) != types::zero;
			val &= ref;
			_cpu._reg_flags[flag_t::Zero] = (val == types::zero); // set zero flag if value is zero
		}
		break;
		default:
			throw std::exception("implement");
		}
		processFlags(carry_flag, overflow_flag, val);
	}

	void cpu_6502_t::InstructionUnit::processFlags(bool carry_flag, bool overflow_flag, Data val)
	{
		using cpu_6502::flag_action_t;
		switch (_inst->flag)
		{
		case flag_action_t::None:
		case flag_action_t::Inst:
			break;
		case flag_action_t::Nrml:
			_cpu._reg_flags[flag_t::Zero] = (val == types::zero);					// set zero flag if value is zero
			_cpu._reg_flags[flag_t::Negative] = (val & kBit7) != types::zero; // set negative flag if value is negative
			_cpu._reg_flags[flag_t::Carry] = carry_flag;
			_cpu._reg_flags[flag_t::Overflow] = overflow_flag;
			break;
		default:
			throw std::exception("implement");
		}
	}

	auto cpu_6502_t::InstructionUnit::storeResult(Data val)
		 -> async::Future<>
	{
		using cpu_6502::data_loc_t;
		switch (_inst->dest)
		{
		case data_loc_t::None:
			break;
		case data_loc_t::Flag:
			_cpu._reg_flags.rawBits() = types::asUnderlyingType(val);
			break;
		case data_loc_t::RegA:
			_cpu._reg_a = val;
			break;
		case data_loc_t::RegX:
			_cpu._reg_x = val;
			break;
		case data_loc_t::RegY:
			_cpu._reg_y = val;
			break;
		case data_loc_t::RegS:
			_cpu._reg_s = val;
			break;
		case data_loc_t::Ptr_:
			co_await _cpu.write_data(_imm_addr, val);
			break;
		case data_loc_t::PtrX:
			co_await _cpu.write_data(_imm_addr + _cpu._reg_x, val);
			break;
		case data_loc_t::PtrY:
			co_await _cpu.write_data(_imm_addr + _cpu._reg_y, val);
			break;
		case data_loc_t::Zpge:
			co_await _cpu.write_data(makeWord(_imm_low, types::zero), val);
			break;
		case data_loc_t::ZppX:
			co_await _cpu.write_data(types::makeWord(_imm_low + _cpu._reg_x, types::zero), val);
			break;
		case data_loc_t::ZppY:
			co_await _cpu.write_data(types::makeWord(_imm_low + _cpu._reg_y, types::zero), val);
			break;
		case data_loc_t::IndY:
			co_await _cpu.write_data(co_await _cpu.read_address_zp(_imm_low) + _cpu._reg_y, val);
			break;
		case data_loc_t::Stck:
			co_await _cpu.push(val);
			break;
		default:
			throw std::exception("implement");
		}
	}

	auto cpu_6502_t::run_inst()
		 -> async::Future<>
	{
#if 1
		InstructionUnit unit(*this);
		co_await unit.fetch();
#else
		using namespace cpu_6502;
		// logger().log("--------------------------");
		auto instCode = co_await read_data(_pc);
		auto& inst = get_instructions()[instCode];
		auto immLow = co_await read_data(_pc + 1);
		Data immHigh = 0;
		Address immAddr = 0;
		if (inst.is_3_byte())
		{
			immHigh = co_await read_data(_pc + 2);
			immAddr = makeword(immLow, immHigh);
			// logger().log("0x%04X: %02X %s(%04X)", _pc, instCode, to_string(inst.opcode), immAddr);
		}
		else
		{
			// logger().log("0x%04X: %02X %s(%02X)", _pc, instCode, to_string(inst.opcode), immLow);
		}
		++_pc;
		if (!inst.is_1_byte())
		{
			++_pc;
			if (inst.is_3_byte())
				++_pc;
		}
#endif
#if 1
		if (co_await unit.handleBranch())
			co_return;
#else
		// group a
		if (inst.is_branch())
		{
			co_await handle_branch((branch_instruction_t)(instCode >> 5), immLow);
			co_return;
		}
		if (inst.is_call())
		{
			co_await handle_call((call_instruction_t)(instCode >> 5), immLow, immHigh);
			co_return;
		}
		if (inst.is_jump())
		{
			co_await handle_jump((jump_instruction_t)((instCode >> 5) & 1), immAddr);
			co_return;
		}
#endif
#if 1
		auto val = co_await unit.loadSource();
#else
		Data val = 0;
		switch (inst.src)
		{
		case data_loc_t::Imm8:
			val = immLow;
			break;
		case data_loc_t::Flag:
			val = _reg_flags.rawBits();
			break;
		case data_loc_t::RegX:
			val = _reg_x;
			break;
		case data_loc_t::RegY:
			val = _reg_y;
			break;
		case data_loc_t::RegA:
			val = _reg_a;
			break;
		case data_loc_t::RegS:
			val = _reg_s;
			break;
		case data_loc_t::PtrX:
			val = co_await read_data(immAddr + _reg_x);
			break;
		case data_loc_t::PtrY:
			val = co_await read_data(immAddr + _reg_y);
			break;
		case data_loc_t::Ptr_:
			val = co_await read_data(immAddr);
			break;
		case data_loc_t::Zpge:
			val = co_await read_data(immLow);
			break;
		case data_loc_t::ZppX:
			val = co_await read_data((immLow + _reg_x) & 0xff);
			break;
		case data_loc_t::ZppY:
			val = co_await read_data((immLow + _reg_y) & 0xff);
			break;
		case data_loc_t::IndY:
			val = co_await read_data(co_await read_address_zp(immLow) + _reg_y);
			break;
		case data_loc_t::Stck:
			val = co_await pop();
			break;
		default:
			throw std::exception("implement");
		}
#endif
#if 1
		auto ref = unit.loadReference();
#else
		Data ref = 0;
		switch (inst.ref)
		{
		case data_loc_t::None:
			break;
		case data_loc_t::Flag:
			ref = _reg_flags.rawBits();
			break;
		case data_loc_t::RegA:
			ref = _reg_a;
			break;
		case data_loc_t::RegX:
			ref = _reg_x;
			break;
		case data_loc_t::RegY:
			ref = _reg_y;
			break;
		case data_loc_t::Bit0:
			ref = 0x01;
			break;
		case data_loc_t::Bit1:
			ref = 0x02;
			break;
		case data_loc_t::Bit2:
			ref = 0x04;
			break;
		case data_loc_t::Bit3:
			ref = 0x08;
			break;
		default:
			throw std::exception("implement");
		}
#endif
#if 1
		unit.execute(val, ref);
#else
		bool carry_flag = _reg_flags[flag_t::Carry];
		bool overflow_flag = _reg_flags[flag_t::Overflow];
		switch (inst.oper)
		{
		case operation_t::Copy:
			break;
		case operation_t::Or__:
			val |= ref;
			break;
		case operation_t::AndN:
			val &= ~ref;
			break;
		case operation_t::And_:
			val &= ref;
			break;
		case operation_t::Sub_: {
			bool signs_equal = (val & 0x80) == (ref & 0x80);
			val = ref - val;
			if (inst.dest != data_loc_t::None)
			{
				val -= (carry_flag ? 0 : 1);
				bool signs_changed = (val & 0x80) == (ref & 0x80);
				overflow_flag = signs_equal && signs_changed;
			}
			carry_flag = (val & 0x80) == 0;
		}
		break;
		case operation_t::Add_: {
			bool signs_equal = (val & 0x80) == (ref & 0x80);
			uint16_t val16 = (uint16_t)ref + (uint16_t)val;
			val = (Data)val16;
			if (inst.dest != data_loc_t::None)
			{
				val += (carry_flag ? 1 : 0);
				bool signs_changed = (val & 0x80) == (ref & 0x80);
				overflow_flag = signs_equal && signs_changed;
			}
			if (_reg_flags[flag_t::Decimal])
				carry_flag = (val16 > 99);
			else
				carry_flag = (val16 & 0xff00) != 0;
		}
		break;
		case operation_t::Dec_:
			val -= ref;
			break;
		case operation_t::Inc_:
			val += ref;
			break;
		case operation_t::Rol_: {
			auto old_carry = carry_flag;
			carry_flag = (val & 0x80) != 0;
			val = (val << 1) | (old_carry ? 1 : 0);
		}
		break;
		case operation_t::Asl_: {
			carry_flag = (val & 0x80) != 0;
			val = (val << 1);
		}
		break;
		case operation_t::Lsr_: {
			carry_flag = (val & 0x1) != 0;
			val = (val >> 1);
		}
		break;
		case operation_t::Xor_:
			val ^= ref;
			break;
		case operation_t::BTst: {
			_reg_flags[flag_t::Negative] = (val & 0x80) != 0; // set negative flag if value is negative
			_reg_flags[flag_t::Overflow] = (val & 0x40) != 0;
			val &= ref;
			_reg_flags[flag_t::Zero] = (val == 0); // set zero flag if value is zero
		}
		break;
		default:
			throw std::exception("implement");
		}
		switch (inst.flag)
		{
		case flag_action_t::None:
			break;
		case flag_action_t::Inst:
			break;
		case flag_action_t::Nrml:
			_reg_flags[flag_t::Zero] = (val == 0);				  // set zero flag if value is zero
			_reg_flags[flag_t::Negative] = (val & 0x80) != 0; // set negative flag if value is negative
			_reg_flags[flag_t::Carry] = carry_flag;
			_reg_flags[flag_t::Overflow] = overflow_flag;
			break;
		default:
			throw std::exception("implement");
		}
#endif
#if 1
		co_await unit.storeResult(val);
#else
		switch (inst.dest)
		{
		case data_loc_t::None:
			break;
		case data_loc_t::Flag:
			_reg_flags.rawBits() = val;
			break;
		case data_loc_t::RegA:
			_reg_a = val;
			break;
		case data_loc_t::RegX:
			_reg_x = val;
			break;
		case data_loc_t::RegY:
			_reg_y = val;
			break;
		case data_loc_t::RegS:
			_reg_s = val;
			break;
		case data_loc_t::Ptr_:
			co_await write_data(immAddr, val);
			break;
		case data_loc_t::PtrX:
			co_await write_data(immAddr + _reg_x, val);
			break;
		case data_loc_t::PtrY:
			co_await write_data(immAddr + _reg_y, val);
			break;
		case data_loc_t::Zpge:
			co_await write_data(immLow, val);
			break;
		case data_loc_t::ZppX:
			co_await write_data((immLow + _reg_x) & 0xff, val);
			break;
		case data_loc_t::ZppY:
			co_await write_data((immLow + _reg_y) & 0xff, val);
			break;
		case data_loc_t::IndY:
			co_await write_data(co_await read_address_zp(immLow) + _reg_y, val);
			break;
		case data_loc_t::Stck:
			co_await push(val);
			break;
		default:
			throw std::exception("implement");
		}
#endif
	}

	constexpr Address kInitialJumpLocation = types::fromUnderlyingType<Address>(0xfffc);

	auto cpu_6502_t::run_cpu()
		 -> async::Future<>
	{
		_pc = co_await read_address(kInitialJumpLocation);
		// logger().log("%s: %d", __FUNCTION__, 0);
		for (;;)
		{
			co_await run_inst();
		}
	}

	auto cpu_6502_t::handle_branch(cpu_6502::Instruction::Branch::Op inst, Data imm_low)
		 -> async::Future<>
	{
		bool take_branch = false;
		using Op = cpu_6502::Instruction::Branch::Op;
		switch (inst)
		{
		case Op::BNE:
			take_branch = !_reg_flags[flag_t::Zero];
			break;
		case Op::BEQ:
			take_branch = _reg_flags[flag_t::Zero];
			break;
		case Op::BCS:
			take_branch = _reg_flags[flag_t::Carry];
			break;
		case Op::BCC:
			take_branch = !_reg_flags[flag_t::Carry];
			break;
		case Op::BMI:
			take_branch = _reg_flags[flag_t::Negative];
			break;
		case Op::BPL:
			take_branch = !_reg_flags[flag_t::Negative];
			break;
		default:
			throw std::exception("implement");
		}
		if (take_branch)
		{
			co_await _clock.nextTick();
			auto page = types::highByte(_pc);
			_pc = types::fromUnderlyingType<Address>(asUnderlyingType(_pc) + static_cast<int8_t>(imm_low));
			auto new_page = types::highByte(_pc);
			if (new_page != page)
				co_await _clock.nextTick();
		}
	}


	auto cpu_6502_t::handle_call(cpu_6502::Instruction::Call::Op inst, Data imm_low, Data imm_high)
		 -> async::Future<>
	{
		using Op = cpu_6502::Instruction::Call::Op;
		switch (inst)
		{
		case Op::JSR: {
			--_pc;
			co_await push(types::highByte(_pc));
			co_await push(types::lowByte(_pc));
			_pc = types::makeWord(imm_low, imm_high);
		}
		break;
		case Op::RTS: {
			auto low = co_await pop();
			auto high = co_await pop();
			_pc = types::makeWord(low, high);
			++_pc;
		}
		break;
		default:
			throw std::exception("implement");
		}
		co_return;
	}

	auto cpu_6502_t::handle_jump(cpu_6502::Instruction::Jump::Op inst, Address imm_addr)
		 -> async::Future<>
	{
		using Op = cpu_6502::Instruction::Jump::Op;
		switch (inst)
		{
		case Op::JMP_Absolute:
			_pc = imm_addr;
			break;
		case Op::JMP_Indirect:
			_pc = co_await read_address(imm_addr);
			break;
		default:
			throw std::exception("implement");
		}
	}

	constexpr Address kStackStart = types::fromUnderlyingType<Address>(0x100);

	auto cpu_6502_t::push(Data data)
		 -> async::Future<>
	{
		auto stack = _reg_s;
		--_reg_s;
		co_await write_data(kStackStart + stack, data);
	}

	auto cpu_6502_t::pop()
		 -> async::Future<Data>
	{
		auto stack = ++_reg_s;
		co_return co_await read_data(kStackStart + stack);
	}

	// for accurate cycle events per instruction: https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf

}
