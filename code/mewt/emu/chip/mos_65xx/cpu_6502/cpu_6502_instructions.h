
#pragma once

#include <stdint.h>
#include <span>

namespace mewt::emu::chip::mos_65xx::cpu_6502
{

	enum class opcode_t
	{
		___, BRK, ORA, ASL, PHP, BPL, CLC, JSR, AND, BIT, ROL, PLP, BMI, SEC, RTI, EOR, LSR, PHA, JMP, BVC, CLI, RTS, ADC, ROR, PLA, BVS, SEI,
		STA, STY, STX, DEY, TXA, BCC, TYA, TXS, LDY, LDA, LDX, TAY, TAX, BCS, CLV, TSX, CPY, CMP, DEC, INY, DEX, BNE, CLD, CPX, SBC, INC, INX,
		NOP, BEQ, SED
	};
	const char* to_string(opcode_t opcode);

	enum class data_loc_t
	{
		____, // unknown
		None,	// no data / data discarded [Implied]
		RegA, // A register [Accumulator]
		RegX, // X register [Implied]
		RegY, // Y register [Implied]
		RegS, // S (stack) register
		Imm8, // 2nd byte of instruction [Immediate]
		//Im16, // 2nd and 3rd bytes of instruction as a 16-bit value { value is 16 bits here }
		Ptr_, // contents of memory at location determined by 2nd and 3rd bytes of instruction [Absolute]
		PtrX, // contents of memory at location determined by adding the X register to the 2nd and 3rd bytes of instruction [X-Indexed Absolute]
		PtrY, // contents of memory at location determined by adding the Y register to the 2nd and 3rd bytes of instruction [Y-Indexed Absolute]
		IndX, // contents of memory at location determined by 16-bit contents of zero page at offset determined by adding the X register to the 2nd byte of instruction [X-Indexed Zero Page Indirect]
		IndY, // contents of memory at location determined by adding the Y register to the 16-bit contents of zero page at offset determined by the 2nd byte of instruction [Zero Page Indirect Y-Indexed]
		ZppX, // contents of zero page at offset determined by adding the X register to the 2nd byte of instruction [X-Indexed Zero Page]
		ZppY, // contents of zero page at offset determined by adding the Y register to the 2nd byte of instruction [Y-Indexed Zero Page]
		Zpge, // contents of zero page at offset determined by the 2nd byte of instruction [Zero Page]
		Stck, // stack - post decrements on write, pre-increments on read
		TODO, // we can possibly be clever about this
		Flag, // Flags register
		Bit0, // = 0x01
		Bit1, // = 0x01
		Bit2, // = 0x04
		Bit3, // = 0x08
		Bit4, // = 0x10
		Bit5, // = 0x20
		Bit6, // = 0x40
		Bit7, // = 0x80
		// PC__, // = program counter
	};

	enum class operation_t
	{
		TODO,
		____,
		Copy,
		Or__,
		And_,
		AndN, // dest = src and not ref
		Sub_,	// sub src from ref. if dest is not none, then alsa subtract ~carry. For normal flags set flags N, Z, C - also set V if dest is not none.
		Add_, // add src to ref. if dest is not none, then also add carry. For normal flags set flags N, Z, C - also set V if dest is not none.
		Dec_, // sub ref from src and place in dest. set only N and Z flags
		Inc_, // add ref to src and place in dest. set only N and Z flags
		Rol_,	// rotate left via the carry flag
		Asl_,	// arithmetic shuift left - top bit out into carry flag
		Lsr_,	// logical shift right - bottom bit out into carry flag
		BTst,	// bit test
		Xor_,
	};

	enum class flag_action_t
	{
		____,
		TODO,
		Nrml,
		None,
		Inst,
	};

	struct Instruction
	{
		uint8_t hex;
		opcode_t opcode = opcode_t::___;
		data_loc_t src = data_loc_t::____;
		data_loc_t dest = data_loc_t::____;
		data_loc_t ref = data_loc_t::____;
		operation_t oper = operation_t::____;
		flag_action_t flag = flag_action_t::____;
		uint8_t size = 0;
		uint8_t cycles = 0;
		bool page_straddle_cycle = false;
		bool branch_taken_cycle = false;
		inline bool is_branch() const { return (hex & 0x1f) == 0x10; }
		inline bool is_call() const { return (hex & 0x9f) == 0; }
		inline bool is_jump() const { return (hex & 0xdf) == 0x4c; }
		inline bool is_1_byte() const { return ((hex & 0xd) == 0x8) || (hex == 0) || (hex == 0x40) || (hex == 0x60); }
		inline bool is_3_byte() const { return ((hex & 0xc) == 0xc) || ((hex & 0x1f) == 0x19) || (hex == 0X20); }
		inline bool is_2_byte() const { return !is_1_byte() && !is_3_byte(); }
		struct Call;
		struct Branch;
		struct Jump;
	};

	std::span<const Instruction> getInstructions();

	struct Instruction::Call
	{
		enum class Op
		{
			BRK,
			JSR,
			RTI,
			RTS,
		};
		static constexpr auto fromInstruction(uint8_t code) -> Op { return static_cast<Op>(code >> 5); }
	};

	struct Instruction::Branch
	{
		enum class Op
		{
			BPL,
			BMI,
			BVC,
			BVS,
			BCC,
			BCS,
			BNE,
			BEQ,
		};
		static constexpr auto fromInstruction(uint8_t code) -> Op { return static_cast<Op>(code >> 5); }
	};

	struct Instruction::Jump
	{
		enum class Op
		{
			JMP_Absolute,
			JMP_Indirect
		};
		static constexpr auto fromInstruction(uint8_t code) -> Op { return static_cast<Op>((code >> 5) & 1); }
	};

}
