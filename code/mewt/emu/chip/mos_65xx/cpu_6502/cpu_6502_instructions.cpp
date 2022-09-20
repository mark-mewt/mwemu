
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"

namespace mewt::emu::chip::mos_65xx::cpu_6502
{

	// https://www.pagetable.com/c64ref/6502/?tab=2#LSR

	using op = opcode_t;
	using from = data_loc_t;
	using to = data_loc_t;
	using with = data_loc_t;
	using do_ = operation_t;
	using flag = flag_action_t;

	const Instruction instructions[] = {

		// _____________________________________________________________________________
		{ 0x00, op::BRK, from::None, to::None, with::None, do_::TODO, flag::TODO, 1, 7 },
		{ 0x01, op::ORA, from::IndX, to::RegA, with::RegA, do_::Or__, flag::Nrml, 2, 6 },
		{ 0x02 },
		{ 0x03 },

		{ 0x04 },
		{ 0x05, op::ORA, from::Zpge, to::RegA, with::RegA, do_::Or__, flag::Nrml, 2, 3 },
		{ 0x06, op::ASL, from::Zpge, to::Zpge, with::None, do_::Asl_, flag::Nrml, 2, 5 },
		{ 0x07 },

		{ 0x08, op::PHP, from::Flag, to::Stck, with::None, do_::Copy, flag::None, 1, 3 },
		{ 0x09, op::ORA, from::Imm8, to::RegA, with::RegA, do_::Or__, flag::Nrml, 2, 2 },
		{ 0x0A, op::ASL, from::RegA, to::RegA, with::None, do_::Asl_, flag::Nrml, 1, 2 },
		{ 0x0B },

		{ 0x0C },
		{ 0x0D, op::ORA, from::Ptr_, to::RegA, with::RegA, do_::Or__, flag::Nrml, 3, 4 },
		{ 0x0E, op::ASL, from::Ptr_, to::Ptr_, with::None, do_::Asl_, flag::Nrml, 3, 6 },
		{ 0x0F },

		// _____________________________________________________________________________
		{ 0x10, op::BPL, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0x11, op::ORA, from::IndY, to::RegA, with::RegA, do_::Or__, flag::Nrml, 2, 5, true  },
		{ 0x12 },
		{ 0x13 },

		{ 0x14 },
		{ 0x15, op::ORA, from::ZppX, to::RegA, with::RegA, do_::Or__, flag::Nrml, 2, 4 },
		{ 0x16, op::ASL, from::ZppX, to::ZppX, with::None, do_::Asl_, flag::Nrml, 2, 6 },
		{ 0x17 },

		{ 0x18, op::CLC, from::Flag, to::Flag, with::Bit0, do_::AndN, flag::None, 1, 2 },
		{ 0x19, op::ORA, from::PtrY, to::RegA, with::RegA, do_::Or__, flag::Nrml, 3, 4, true },
		{ 0x1A },
		{ 0x1B },

		{ 0x1C },
		{ 0x1D, op::ORA, from::PtrX, to::RegA, with::RegA, do_::Or__, flag::Nrml, 3, 4, true },
		{ 0x1E, op::ASL, from::PtrX, to::PtrX, with::None, do_::Asl_, flag::Nrml, 3, 7 },
		{ 0x1F },

		// _____________________________________________________________________________
		{ 0x20, op::JSR, from::None, to::None, with::None, do_::TODO, flag::None, 3, 6 },
		{ 0x21, op::AND, from::IndX, to::RegA, with::RegA, do_::TODO, flag::Nrml, 2, 6 },
		{ 0x22 },
		{ 0x23 },

		{ 0x24, op::BIT, from::Zpge, to::None, with::RegA, do_::BTst, flag::Inst, 2, 3 },
		{ 0x25, op::AND, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::Nrml, 2, 3 },
		{ 0x26, op::ROL, from::Zpge, to::Zpge, with::None, do_::Rol_, flag::Nrml, 2, 5 },
		{ 0x27 },

		{ 0x28, op::PLP, from::Stck, to::Flag, with::None, do_::Copy, flag::None, 1, 4 },
		{ 0x29, op::AND, from::Imm8, to::RegA, with::RegA, do_::And_, flag::Nrml, 2, 2 },
		{ 0x2A, op::ROL, from::RegA, to::RegA, with::None, do_::Rol_, flag::Nrml, 1, 2 },
		{ 0x2B },

		{ 0x2C, op::BIT, from::Ptr_, to::None, with::RegA, do_::BTst, flag::Inst, 3, 4 },
		{ 0x2D, op::AND, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::Nrml, 3, 4 },
		{ 0x2E, op::ROL, from::Ptr_, to::Ptr_, with::None, do_::Rol_, flag::Nrml, 3, 6 },
		{ 0x2F },

		// _____________________________________________________________________________
		{ 0x30, op::BMI, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0x31, op::AND, from::IndY, to::RegA, with::RegA, do_::TODO, flag::Nrml, 2, 5, true },
		{ 0x32 },
		{ 0x33 },

		{ 0x34 },
		{ 0x35, op::AND, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::Nrml, 2, 4 },
		{ 0x36, op::ROL, from::ZppX, to::ZppX, with::None, do_::Rol_, flag::Nrml, 2, 6 },
		{ 0x37 },

		{ 0x38, op::SEC, from::Flag, to::Flag, with::Bit0, do_::Or__, flag::None, 1, 2 },
		{ 0x39, op::AND, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::Nrml, 3, 4, true },
		{ 0x3A },
		{ 0x3B },

		{ 0x3C },
		{ 0x3D, op::AND, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::Nrml, 3, 4, true },
		{ 0x3E, op::ROL, from::PtrX, to::PtrX, with::None, do_::Rol_, flag::Nrml, 3, 7 },
		{ 0x3F },

		// _____________________________________________________________________________
		{ 0x40, op::RTI, from::None, to::None, with::None, do_::TODO, flag::Inst, 1, 6 },
		{ 0x41, op::EOR, from::IndX, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 2, 6 },
		{ 0x42 },
		{ 0x43 },

		{ 0x44 },
		{ 0x45, op::EOR, from::Zpge, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 2, 3 },
		{ 0x46, op::LSR, from::Zpge, to::Zpge, with::None, do_::Lsr_, flag::Nrml, 2, 5 },
		{ 0x47 },

		{ 0x48, op::PHA, from::RegA, to::Stck, with::None, do_::Copy, flag::None, 1, 3 },
		{ 0x49, op::EOR, from::Imm8, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 2, 2 },
		{ 0x4A, op::LSR, from::RegA, to::RegA, with::None, do_::Lsr_, flag::Nrml, 1, 2 },
		{ 0x4B },

		{ 0x4C, op::JMP, from::None, to::None, with::None, do_::TODO, flag::None, 3, 3 },
		{ 0x4D, op::EOR, from::Ptr_, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 3, 4 },
		{ 0x4E, op::LSR, from::Ptr_, to::Ptr_, with::None, do_::Lsr_, flag::Nrml, 3, 6 },
		{ 0x4F },

		// _____________________________________________________________________________
		{ 0x50, op::BVC, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0x51, op::EOR, from::IndY, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 2, 5, true },
		{ 0x52 },
		{ 0x53 },

		{ 0x54 },
		{ 0x55, op::EOR, from::ZppX, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 2, 4 },
		{ 0x56, op::LSR, from::ZppX, to::ZppX, with::None, do_::Lsr_, flag::Nrml, 2, 6 },
		{ 0x57 },

		{ 0x58, op::CLI, from::Flag, to::Flag, with::Bit2, do_::AndN, flag::None, 1, 2 },
		{ 0x59, op::EOR, from::PtrY, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 3, 4, true },
		{ 0x5A },
		{ 0x5B },

		{ 0x5C },
		{ 0x5D, op::EOR, from::PtrX, to::RegA, with::RegA, do_::Xor_, flag::Nrml, 3, 4, true },
		{ 0x5E, op::LSR, from::PtrX, to::PtrX, with::None, do_::Lsr_, flag::Nrml, 3, 7 },
		{ 0x5F },

		// _____________________________________________________________________________
		{ 0x60, op::RTS, from::None, to::None, with::None, do_::TODO, flag::None, 1, 6 },
		{ 0x61, op::ADC, from::IndX, to::RegA, with::RegA, do_::Add_, flag::Nrml, 2, 6 },
		{ 0x62 },
		{ 0x63 },

		{ 0x64 },
		{ 0x65, op::ADC, from::Zpge, to::RegA, with::RegA, do_::Add_, flag::Nrml, 2, 3 },
		{ 0x66, op::ROR, from::Zpge, to::Zpge, with::None, do_::TODO, flag::Nrml, 2, 5 },
		{ 0x67 },

		{ 0x68, op::PLA, from::Stck, to::RegA, with::None, do_::Copy, flag::None, 1, 4 },
		{ 0x69, op::ADC, from::Imm8, to::RegA, with::RegA, do_::Add_, flag::Nrml, 2, 2 },
		{ 0x6A, op::ROR, from::RegA, to::RegA, with::None, do_::TODO, flag::Nrml, 1, 2 },
		{ 0x6B },

		{ 0x6C, op::JMP, from::None, to::None, with::None, do_::TODO, flag::None, 3, 5 },
		{ 0x6D, op::ADC, from::Ptr_, to::RegA, with::RegA, do_::Add_, flag::Nrml, 3, 4 },
		{ 0x6E, op::ROR, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::Nrml, 3, 6 },
		{ 0x6F },

		// _____________________________________________________________________________
		{ 0x70, op::BVS, from::Imm8, to::TODO, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0x71, op::ADC, from::IndY, to::RegA, with::RegA, do_::Add_, flag::Nrml, 2, 5, true },
		{ 0x72 },
		{ 0x73 },

		{ 0x74 },
		{ 0x75, op::ADC, from::ZppX, to::RegA, with::RegA, do_::Add_, flag::Nrml, 2, 4 },
		{ 0x76, op::ROR, from::ZppX, to::ZppX, with::None, do_::TODO, flag::Nrml, 2, 6 },
		{ 0x77 },

		{ 0x78, op::SEI, from::Flag, to::Flag, with::Bit2, do_::Or__, flag::None, 1, 2 },
		{ 0x79, op::ADC, from::PtrY, to::RegA, with::RegA, do_::Add_, flag::Nrml, 3, 4, true },
		{ 0x7A },
		{ 0x7B },

		{ 0x7C },
		{ 0x7D, op::ADC, from::PtrX, to::RegA, with::RegA, do_::Add_, flag::Nrml, 3, 4, true },
		{ 0x7E, op::ROR, from::PtrX, to::PtrX, with::None, do_::TODO, flag::Nrml, 3, 7 },
		{ 0x7F },

		// _____________________________________________________________________________
		{ 0x80 },
		{ 0x81, op::STA, from::RegA, to::IndX, with::None, do_::Copy, flag::None, 2, 6 },
		{ 0x82 },
		{ 0x83 },

		{ 0x84, op::STY, from::RegY, to::Zpge, with::None, do_::Copy, flag::None, 2, 3 },
		{ 0x85, op::STA, from::RegA, to::Zpge, with::None, do_::Copy, flag::None, 2, 3 },
		{ 0x86, op::STX, from::RegX, to::Zpge, with::None, do_::Copy, flag::None, 2, 3 },
		{ 0x87 },

		{ 0x88, op::DEY, from::RegY, to::RegY, with::Bit0, do_::Dec_, flag::Nrml, 1, 2 },
		{ 0x89 },
		{ 0x8A, op::TXA, from::RegX, to::RegA, with::None, do_::Copy, flag::Nrml, 1, 2 },
		{ 0x8B },

		{ 0x8C, op::STY, from::RegY, to::Ptr_, with::None, do_::Copy, flag::None, 3, 4 },
		{ 0x8D, op::STA, from::RegA, to::Ptr_, with::None, do_::Copy, flag::None, 3, 4 },
		{ 0x8E, op::STX, from::RegX, to::Ptr_, with::None, do_::Copy, flag::None, 3, 4 },
		{ 0x8F },

		// _____________________________________________________________________________
		{ 0x90, op::BCC, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0x91, op::STA, from::RegA, to::IndY, with::None, do_::Copy, flag::None, 2, 6 },
		{ 0x92 },
		{ 0x93 },

		{ 0x94, op::STY, from::RegY, to::ZppX, with::None, do_::Copy, flag::None, 2, 4 },
		{ 0x95, op::STA, from::RegA, to::ZppX, with::None, do_::Copy, flag::None, 2, 4 },
		{ 0x96, op::STX, from::RegX, to::ZppY, with::None, do_::Copy, flag::None, 2, 4 },
		{ 0x97 },

		{ 0x98, op::TYA, from::RegY, to::RegA, with::None, do_::Copy, flag::Nrml, 1, 2 },
		{ 0x99, op::STA, from::RegA, to::PtrY, with::None, do_::Copy, flag::None, 3, 5 },
		{ 0x9A, op::TXS, from::RegX, to::RegS, with::None, do_::Copy, flag::None, 1, 2 },
		{ 0x9B },

		{ 0x9C },
		{ 0x9D, op::STA, from::RegA, to::PtrX, with::None, do_::Copy, flag::None, 3, 5 },
		{ 0x9E },
		{ 0x9F },

		// _____________________________________________________________________________
		{ 0xA0, op::LDY, from::Imm8, to::RegY, with::None, do_::Copy, flag::Nrml, 2, 2 },
		{ 0xA1, op::LDA, from::IndX, to::RegA, with::None, do_::Copy, flag::Nrml, 2, 6 },
		{ 0xA2, op::LDX, from::Imm8, to::RegX, with::None, do_::Copy, flag::Nrml, 2, 2 },
		{ 0xA3 },

		{ 0xA4, op::LDY, from::Zpge, to::RegY, with::None, do_::Copy, flag::Nrml, 2, 3 },
		{ 0xA5, op::LDA, from::Zpge, to::RegA, with::None, do_::Copy, flag::Nrml, 2, 3 },
		{ 0xA6, op::LDX, from::Zpge, to::RegX, with::None, do_::Copy, flag::Nrml, 2, 3 },
		{ 0xA7 },

		{ 0xA8, op::TAY, from::RegA, to::RegY, with::None, do_::Copy, flag::Nrml, 1, 2 },
		{ 0xA9, op::LDA, from::Imm8, to::RegA, with::None, do_::Copy, flag::Nrml, 2, 2 },
		{ 0xAA, op::TAX, from::RegA, to::RegX, with::None, do_::Copy, flag::Nrml, 1, 2 },
		{ 0xAB },

		{ 0xAC, op::LDY, from::Ptr_, to::RegY, with::None, do_::Copy, flag::Nrml, 3, 4 },
		{ 0xAD, op::LDA, from::Ptr_, to::RegA, with::None, do_::Copy, flag::Nrml, 3, 4 },
		{ 0xAE, op::LDX, from::Ptr_, to::RegX, with::None, do_::Copy, flag::Nrml, 3, 4 },
		{ 0xAF },

		// _____________________________________________________________________________
		{ 0xB0, op::BCS, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0xB1, op::LDA, from::IndY, to::RegA, with::None, do_::Copy, flag::Nrml, 2, 5, true },
		{ 0xB2 },
		{ 0xB3 },

		{ 0xB4, op::LDY, from::ZppX, to::RegY, with::None, do_::Copy, flag::Nrml, 2, 4 },
		{ 0xB5, op::LDA, from::ZppX, to::RegA, with::None, do_::Copy, flag::Nrml, 2, 4 },
		{ 0xB6, op::LDX, from::ZppY, to::RegX, with::None, do_::Copy, flag::Nrml, 2, 4 },
		{ 0xB7 },

		{ 0xB8, op::CLV, from::Flag, to::Flag, with::Bit6, do_::AndN, flag::None, 1, 2 },
		{ 0xB9, op::LDA, from::PtrY, to::RegA, with::None, do_::Copy, flag::Nrml, 3, 4, true },
		{ 0xBA, op::TSX, from::RegS, to::RegX, with::None, do_::Copy, flag::Nrml, 1, 2 },
		{ 0xBB },

		{ 0xBC, op::LDY, from::PtrX, to::RegY, with::None, do_::Copy, flag::Nrml, 3, 4, true },
		{ 0xBD, op::LDA, from::PtrX, to::RegA, with::None, do_::Copy, flag::Nrml, 3, 4, true },
		{ 0xBE, op::LDX, from::PtrY, to::RegX, with::None, do_::Copy, flag::Nrml, 3, 4, true },
		{ 0xBF },

		// _____________________________________________________________________________
		{ 0xC0, op::CPY, from::Imm8, to::None, with::RegY, do_::Sub_, flag::Nrml, 2, 2 },
		{ 0xC1, op::CMP, from::IndX, to::None, with::RegA, do_::Sub_, flag::Nrml, 2, 6 },
		{ 0xC2 },
		{ 0xC3 },

		{ 0xC4, op::CPY, from::Zpge, to::None, with::RegY, do_::Sub_, flag::Nrml, 2, 3 },
		{ 0xC5, op::CMP, from::Zpge, to::None, with::RegA, do_::Sub_, flag::Nrml, 2, 3 },
		{ 0xC6, op::DEC, from::Zpge, to::Zpge, with::Bit0, do_::Dec_, flag::Nrml, 2, 5 },
		{ 0xC7 },

		{ 0xC8, op::INY, from::RegY, to::RegY, with::Bit0, do_::Inc_, flag::Nrml, 1, 2 },
		{ 0xC9, op::CMP, from::Imm8, to::None, with::RegA, do_::Sub_, flag::Nrml, 2, 2 },
		{ 0xCA, op::DEX, from::RegX, to::RegX, with::Bit0, do_::Dec_, flag::Nrml, 1, 2 },
		{ 0xCB },

		{ 0xCC, op::CPY, from::Ptr_, to::None, with::RegY, do_::Sub_, flag::Nrml, 3, 4 },
		{ 0xCD, op::CMP, from::Ptr_, to::None, with::RegA, do_::Sub_, flag::Nrml, 3, 4 },
		{ 0xCE, op::DEC, from::Ptr_, to::Ptr_, with::Bit0, do_::Dec_, flag::Nrml, 3, 6 },
		{ 0xCF },

		// _____________________________________________________________________________
		{ 0xD0, op::BNE, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0xD1, op::CMP, from::IndY, to::None, with::RegA, do_::Sub_, flag::Nrml, 2, 5, true },
		{ 0xD2 },
		{ 0xD3 },

		{ 0xD4 },
		{ 0xD5, op::CMP, from::ZppX, to::None, with::RegA, do_::Sub_, flag::Nrml, 2, 4 },
		{ 0xD6, op::DEC, from::ZppX, to::ZppX, with::Bit0, do_::Dec_, flag::Nrml, 2, 6 },
		{ 0xD7 },

		{ 0xD8, op::CLD, from::Flag, to::Flag, with::Bit3, do_::AndN, flag::None, 1, 2 },
		{ 0xD9, op::CMP, from::PtrY, to::None, with::RegA, do_::Sub_, flag::Nrml, 3, 4, true },
		{ 0xDA },
		{ 0xDB },

		{ 0xDC },
		{ 0xDD, op::CMP, from::PtrX, to::None, with::RegA, do_::Sub_, flag::Nrml, 3, 4, true },
		{ 0xDE, op::DEC, from::PtrX, to::PtrX, with::Bit0, do_::Dec_, flag::Nrml, 3, 7 },
		{ 0xDF },

		// _____________________________________________________________________________
		{ 0xE0, op::CPX, from::Imm8, to::None, with::RegX, do_::Sub_, flag::Nrml, 2, 2 },
		{ 0xE1, op::SBC, from::IndX, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 2, 6 },
		{ 0xE2 },
		{ 0xE3 },

		{ 0xE4, op::CPX, from::Zpge, to::None, with::RegX, do_::Sub_, flag::Nrml, 2, 3 },
		{ 0xE5, op::SBC, from::Zpge, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 2, 3 },
		{ 0xE6, op::INC, from::Zpge, to::Zpge, with::Bit0, do_::Inc_, flag::Nrml, 2, 5 },
		{ 0xE7 },

		{ 0xE8, op::INX, from::RegX, to::RegX, with::Bit0, do_::Inc_, flag::Nrml, 1, 2 },
		{ 0xE9, op::SBC, from::Imm8, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 2, 2 },
		{ 0xEA, op::NOP, from::None, to::None, with::None, do_::TODO, flag::None, 1, 2 },
		{ 0xEB },

		{ 0xEC, op::CPX, from::Ptr_, to::None, with::RegX, do_::Sub_, flag::Nrml, 3, 4 },
		{ 0xED, op::SBC, from::Ptr_, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 3, 4 },
		{ 0xEE, op::INC, from::Ptr_, to::Ptr_, with::Bit0, do_::Inc_, flag::Nrml, 3, 6 },
		{ 0xEF },

		// _____________________________________________________________________________
		{ 0xF0, op::BEQ, from::Imm8, to::None, with::None, do_::TODO, flag::None, 2, 2, true, true },
		{ 0xF1, op::SBC, from::IndY, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 2, 5, true },
		{ 0xF2 },
		{ 0xF3 },

		{ 0xF4 },
		{ 0xF5, op::SBC, from::ZppX, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 2, 4 },
		{ 0xF6, op::INC, from::ZppX, to::ZppX, with::None, do_::TODO, flag::Nrml, 2, 6 },
		{ 0xF7 },

		{ 0xF8, op::SED, from::TODO, to::TODO, with::TODO, do_::TODO, flag::Inst, 1, 2 },
		{ 0xF9, op::SBC, from::PtrY, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 3, 4, true },
		{ 0xFA },
		{ 0xFB },

		{ 0xFC },
		{ 0xFD, op::SBC, from::PtrX, to::RegA, with::RegA, do_::Sub_, flag::Nrml, 3, 4, true },
		{ 0xFE, op::INC, from::PtrX, to::PtrX, with::None, do_::TODO, flag::Nrml, 3, 7 },
		{ 0xFF },

	};

	/*

		group A = (x & 0x1f) == 0x10 => branch instructions
		{ 0x10, op::BPL, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	Plus	-	2	2, true, true
		{ 0x30, op::BMI, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	Minus	-	2	2, true, true
		{ 0x50, op::BVC, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	Overflow Clear	-	2	2, true, true
		{ 0x70, op::BVS, from::Imm8, to::TODO, with::None, do_::TODO, flag::TODO }, //	Branch	OverflowSet	-	2	2, true, true
		{ 0x90, op::BCC, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	CarryClear	-	2	2, true, true
		{ 0xB0, op::BCS, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	CarrySet	-	2	2, true, true
		{ 0xD0, op::BNE, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	NotEqual	-	2	2, true, true
		{ 0xF0, op::BEQ, from::Imm8, to::None, with::None, do_::TODO, flag::TODO }, //	Branch	Equal	-	2	2, true, true

		group B = (x & 0x9f) == 0 => call instructions
		{ 0x00, op::BRK, from::None, to::None, with::None, do_::TODO, flag::TODO }, //	Break		B, true	7
		{ 0x20, op::JSR, from::Im16, to::TODO, with::None, do_::TODO, flag::TODO }, //	JSR		?	3	6			Jump to subroutine
		{ 0x40, op::RTI, from::None, to::None, with::None, do_::TODO, flag::TODO }, //	ReturnInt	-	All, true	6			Return from interrupt
		{ 0x60, op::RTS, from::None, to::None, with::None, do_::TODO, flag::TODO }, //	ReturnSub	-	-, true	6			Return from Subroutine


		group Z = everything else
		-----
		{ 0x01, op::ORA, from::IndX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	2	6
		{ 0x02, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x03, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x04, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x05, op::ORA, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	2	3
		{ 0x06, op::ASL, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	ASL1		N,Z,C	2	5
		{ 0x07, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x08, op::PHP, from::Stus, to::Stck, with::None, do_::TODO, flag::TODO }, //	-	-	-, true	3
		{ 0x09, op::ORA, from::Imm8, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	2	2
		{ 0x0A, op::ASL, from::RegA, to::RegA, with::None, do_::TODO, flag::TODO }, //	ASL1		N,Z,C, true	2
		{ 0x0B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x0C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x0D, op::ORA, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	3	4
		{ 0x0E, op::ASL, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	ASL1		N,Z,C	3	6
		{ 0x0F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x11, op::ORA, from::IndY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	2	5, true
		{ 0x12, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x13, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x14, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x15, op::ORA, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	2	4
		{ 0x16, op::ASL, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	ASL1		N,Z,C	2	6
		{ 0x17, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x18, op::CLC, from::TODO, to::TODO, with::TODO, do_::TODO, flag::TODO }, // 	C, true	2			Clear Carry
		{ 0x19, op::ORA, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	3	4, true
		{ 0x1A, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x1B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x1C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x1D, op::ORA, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Or	-	N,Z	3	4, true
		{ 0x1E, op::ASL, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	ASL1		N,Z,C	3	7
		{ 0x1F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x21, op::AND, from::IndX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	AndWithDest		N,Z	2	6
		{ 0x22, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x23, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x24, op::BIT, from::Zpge, to::None, with::RegA, do_::TODO, flag::TODO }, //	BitTest		N,V,Z	2	3			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x25, op::AND, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	2	3
		{ 0x26, op::ROL, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	RotateLeft		N,Z,C	2	5
		{ 0x27, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x28, op::PLP, from::Stck, to::Stus, with::None, do_::TODO, flag::TODO }, //	-	-	-, true	4
		{ 0x29, op::AND, from::Imm8, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	2	2
		{ 0x2A, op::ROL, from::RegA, to::RegA, with::None, do_::TODO, flag::TODO }, //	RotateLeft		N,Z,C, true	2
		{ 0x2B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x2C, op::BIT, from::Ptr_, to::None, with::RegA, do_::TODO, flag::TODO }, //	BitTest		N,V,Z	3	4			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x2D, op::AND, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	3	4
		{ 0x2E, op::ROL, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	RotateLeft		N,Z,C	3	6
		{ 0x2F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x31, op::AND, from::IndY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	2	5, true
		{ 0x32, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x33, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x34, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x35, op::AND, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	2	4
		{ 0x36, op::ROL, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	RotateLeft		N,Z,C	2	6
		{ 0x37, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x38, op::SEC, from::TODO, to::TODO, with::TODO, do_::TODO, flag::TODO }, // 	C, true	2			Set Carry
		{ 0x39, op::AND, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	3	4, true
		{ 0x3A, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x3B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x3C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x3D, op::AND, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add		N,Z	3	4, true
		{ 0x3E, op::ROL, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	RotateLeft		N,Z,C	3	7
		{ 0x3F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x41, op::EOR, from::IndX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	2	6
		{ 0x42, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x43, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x44, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x45, op::EOR, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	2	3
		{ 0x46, op::LSR, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	Lsr	-	N,Z,C	2	5
		{ 0x47, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x48, op::PHA, from::RegA, to::Stck, with::None, do_::TODO, flag::TODO }, //	-	-	-, true	3
		{ 0x49, op::EOR, from::Imm8, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	2	2
		{ 0x4A, op::LSR, from::RegA, to::RegA, with::None, do_::TODO, flag::TODO }, //	Lsr	-	N,Z,C, true	2
		{ 0x4B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x4C, op::JMP, from::Im16, to::TODO, with::None, do_::TODO, flag::TODO }, //	Branch	-	-	3	3
		{ 0x4D, op::EOR, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	3	4
		{ 0x4E, op::LSR, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	Lsr	-	N,Z,C	3	6
		{ 0x4F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x51, op::EOR, from::IndY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	2	5, true
		{ 0x52, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x53, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x54, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x55, op::EOR, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	2	4
		{ 0x56, op::LSR, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	Lsr	-	N,Z,C	2	6
		{ 0x57, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x58, op::CLI, from::TODO, to::TODO, with::TODO, do_::TODO, flag::TODO }, // 	I, true	2			Clear Interrupt
		{ 0x59, op::EOR, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	3	4, true
		{ 0x5A, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x5B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x5C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x5D, op::EOR, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Xor		N,Z	3	4, true
		{ 0x5E, op::LSR, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	Lsr	-	N,Z,C	3	7
		{ 0x5F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x61, op::ADC, from::IndX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	2	6
		{ 0x62, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x63, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x64, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x65, op::ADC, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	2	3
		{ 0x66, op::ROR, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	RotateRight		N,Z,C	2	5
		{ 0x67, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x68, op::PLA, from::Stck, to::RegA, with::None, do_::TODO, flag::TODO }, //	-	-	-, true	4
		{ 0x69, op::ADC, from::Imm8, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	2	2
		{ 0x6A, op::ROR, from::RegA, to::RegA, with::None, do_::TODO, flag::TODO }, //	RotateRight		N,Z,C, true	2
		{ 0x6B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x6C, op::JMP, from::Ptr_, to::TODO, with::None, do_::TODO, flag::TODO }, //		Branch	-	-	3	5
		{ 0x6D, op::ADC, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	3	4
		{ 0x6E, op::ROR, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	RotateRight		N,Z,C	3	6
		{ 0x6F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x71, op::ADC, from::IndY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	2	5, true
		{ 0x72, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x73, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x74, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x75, op::ADC, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	2	4
		{ 0x76, op::ROR, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	RotateRight		N,Z,C	2	6
		{ 0x77, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x78, op::SEI, from::Flag, to::Flag, with::Bit2, do_::Or__, flag::None }, // 	I, true	2			Set Interrupt
		{ 0x79, op::ADC, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	3	4, true
		{ 0x7A, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x7B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x7C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x7D, op::ADC, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	Add2DstC		N,V,Z,C	3	4, true
		{ 0x7E, op::ROR, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	RotateRight		N,Z,C	3	7
		{ 0x7F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x80, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x81, op::STA, from::RegA, to::IndX, with::None, do_::TODO, flag::TODO }, //	-		-	2	6
		{ 0x82, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x83, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x84, op::STY, from::RegY, to::Zpge, with::None, do_::TODO, flag::TODO }, //	-	-	-	2	3
		{ 0x85, op::STA, from::RegA, to::Zpge, with::None, do_::TODO, flag::TODO }, //	-		-	2	3
		{ 0x86, op::STX, from::RegX, to::Zpge, with::None, do_::TODO, flag::TODO }, //	-	-	-	2	3
		{ 0x87, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x88, op::DEY, from::RegY, to::RegY, with::None, do_::TODO, flag::TODO }, //	Dec		N,Z, true	2
		{ 0x89, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x8A, op::TXA, from::RegX, to::RegA, with::None, do_::TODO, flag::TODO }, //			N,Z, true	2
		{ 0x8B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x8C, op::STY, from::RegY, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	-	-	-	3	4
		{ 0x8D, op::STA, from::RegA, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	-		-	3	4
		{ 0x8E, op::STX, from::RegX, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	-	-	-	3	4
		{ 0x8F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0x91, op::STA, from::RegA, to::IndY, with::None, do_::TODO, flag::TODO }, //	-		-	2	6
		{ 0x92, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x93, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x94, op::STY, from::RegY, to::ZppX, with::None, do_::TODO, flag::TODO }, //	-	-	-	2	4
		{ 0x95, op::STA, from::RegA, to::ZppX, with::None, do_::TODO, flag::TODO }, //	-		-	2	4
		{ 0x96, op::STX, from::RegX, to::ZppY, with::None, do_::TODO, flag::TODO }, //	-	-	-	2	4
		{ 0x97, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x98, op::TYA, from::RegY, to::RegA, with::None, do_::TODO, flag::TODO }, //			N,Z, true	2
		{ 0x99, op::STA, from::RegA, to::PtrY, with::None, do_::TODO, flag::TODO }, //	-		-	3	5
		{ 0x9A, op::TXS, from::RegX, to::RegS, with::None, do_::Copy, flag::None }, //	-	-	-, true	2
		{ 0x9B, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x9C, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x9D, op::STA, from::RegA, to::PtrX, with::None, do_::TODO, flag::TODO }, //	-		-	3	5
		{ 0x9E, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0x9F, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xA0, op::LDY, from::Imm8, to::RegY, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	2			Load Y Immediate
		{ 0xA1, op::LDA, from::IndX, to::RegA, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	6
		{ 0xA2, op::LDX, from::Imm8, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	2			Load X Immediate
		{ 0xA3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xA4, op::LDY, from::Zpge, to::RegY, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	3
		{ 0xA5, op::LDA, from::Zpge, to::RegA, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	3
		{ 0xA6, op::LDX, from::Zpge, to::RegX, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	3
		{ 0xA7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xA8, op::TAY, from::RegA, to::RegY, with::None, do_::TODO, flag::TODO }, //			N,Z, true	2
		{ 0xA9, op::LDA, from::Imm8, to::RegA, with::None, do_::TODO, flag::TODO }, //	-		N,Z	2	2			Load Accumulator Immediate
		{ 0xAA, op::TAX, from::RegA, to::RegX, with::None, do_::TODO, flag::TODO }, //			N,Z, true	2
		{ 0xAB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xAC, op::LDY, from::Ptr_, to::RegY, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4
		{ 0xAD, op::LDA, from::Ptr_, to::RegA, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4
		{ 0xAE, op::LDX, from::Ptr_, to::RegX, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4
		{ 0xAF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0xB1, op::LDA, from::IndY, to::RegA, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	2	5, true
		{ 0xB2, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xB3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xB4, op::LDY, from::ZppX, to::RegY, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	2	4
		{ 0xB5, op::LDA, from::ZppX, to::RegA, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	2	4
		{ 0xB6, op::LDX, from::ZppY, to::RegX, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	2	4
		{ 0xB7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xB8, op::CLV, from::TODO, to::TODO, with::TODO, do_::TODO, flag::TODO }, // 			O, true	2			Clear Overflow
		{ 0xB9, op::LDA, from::PtrY, to::RegA, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4, true
		{ 0xBA, op::TSX, from::RegS, to::RegX, with::None, do_::TODO, flag::TODO }, //	-	- 	-, true	2
		{ 0xBB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xBC, op::LDY, from::PtrX, to::RegY, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4, true
		{ 0xBD, op::LDA, from::PtrX, to::RegA, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4, true
		{ 0xBE, op::LDX, from::PtrY, to::RegX, with::None, do_::TODO, flag::TODO }, //	- 		N,Z	3	4, true
		{ 0xBF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xC0, op::CPY, from::Imm8, to::None, with::RegY, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	2
		{ 0xC1, op::CMP, from::IndX, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	6
		{ 0xC2, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xC3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xC4, op::CPY, from::Zpge, to::None, with::RegY, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	3
		{ 0xC5, op::CMP, from::Zpge, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	3
		{ 0xC6, op::DEC, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	Dec 	N,Z	2	5
		{ 0xC7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xC8, op::INY, from::RegY, to::RegY, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z, true	2
		{ 0xC9, op::CMP, from::Imm8, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	2
		{ 0xCA, op::DEX, from::RegX, to::RegX, with::None, do_::TODO, flag::TODO }, //	Dec 	N,Z, true	2
		{ 0xCB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xCC, op::CPY, from::Ptr_, to::None, with::RegY, do_::TODO, flag::TODO }, //	Compare N,Z,C	3	4
		{ 0xCD, op::CMP, from::Ptr_, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	3	4
		{ 0xCE, op::DEC, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	Dec 	N,Z	3	6
		{ 0xCF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0xD1, op::CMP, from::IndY, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	5, true
		{ 0xD2, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xD3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xD4, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xD5, op::CMP, from::ZppX, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	4
		{ 0xD6, op::DEC, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	Dec 	N,Z	2	6
		{ 0xD7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xD8, op::CLD, from::Flag, to::Flag, with::Bit3, do_::AndN, flag::None }, // 			D, true	2			Clear Decimal
		{ 0xD9, op::CMP, from::PtrY, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	3	4, true
		{ 0xDA, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xDB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xDC, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xDD, op::CMP, from::PtrX, to::None, with::RegA, do_::TODO, flag::TODO }, //	Compare N,Z,C	3	4, true
		{ 0xDE, op::DEC, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	Dec 	N,Z	3	7
		{ 0xDF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xE0, op::CPX, from::Imm8, to::None, with::RegX, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	2
		{ 0xE1, op::SBC, from::IndX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	2	6
		{ 0xE2, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xE3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xE4, op::CPX, from::Zpge, to::None, with::RegX, do_::TODO, flag::TODO }, //	Compare N,Z,C	2	3
		{ 0xE5, op::SBC, from::Zpge, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	2	3
		{ 0xE6, op::INC, from::Zpge, to::Zpge, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z	2	5
		{ 0xE7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xE8, op::INX, from::RegX, to::RegX, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z, true	2
		{ 0xE9, op::SBC, from::Imm8, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	2	2
		{ 0xEA, op::NOP, from::None, to::None, with::None, do_::TODO, flag::TODO }, //			-	-			-, true	2
		{ 0xEB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xEC, op::CPX, from::Ptr_, to::None, with::RegX, do_::TODO, flag::TODO }, //	Compare N,Z,C	3	4
		{ 0xED, op::SBC, from::Ptr_, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	3	4
		{ 0xEE, op::INC, from::Ptr_, to::Ptr_, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z	3	6
		{ 0xEF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		-----
		{ 0xF1, op::SBC, from::IndY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	2	5, true
		{ 0xF2, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xF3, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xF4, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xF5, op::SBC, from::ZppX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	2	4
		{ 0xF6, op::INC, from::ZppX, to::ZppX, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z	2	6
		{ 0xF7, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xF8, op::SED, from::TODO, to::TODO, with::TODO, do_::TODO, flag::TODO }, // 			D, true	2			Set Decimal
		{ 0xF9, op::SBC, from::PtrY, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	3	4, true
		{ 0xFA, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xFB, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xFC, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //
		{ 0xFD, op::SBC, from::PtrX, to::RegA, with::RegA, do_::TODO, flag::TODO }, //	SubWithCarry	-	N,V,Z,C	3	4, true
		{ 0xFE, op::INC, from::PtrX, to::PtrX, with::None, do_::TODO, flag::TODO }, //	Inc 	N,Z	3	7
		{ 0xFF, op::___, from::____, to::____, with::____, do_::____, flag::____ }, //


	*/

	std::span<const Instruction> getInstructions()
	{
		return instructions;
	}

	const char* to_string(opcode_t opcode)
	{
#define HANDLE(s) case opcode_t::s: return #s;
		switch (opcode)
		{
			HANDLE(___);
			HANDLE(BRK);
			HANDLE(ORA);
			HANDLE(ASL);
			HANDLE(PHP);
			HANDLE(BPL);
			HANDLE(CLC);
			HANDLE(JSR);
			HANDLE(AND);
			HANDLE(BIT);
			HANDLE(ROL);
			HANDLE(PLP);
			HANDLE(BMI);
			HANDLE(SEC);
			HANDLE(RTI);
			HANDLE(EOR);
			HANDLE(LSR);
			HANDLE(PHA);
			HANDLE(JMP);
			HANDLE(BVC);
			HANDLE(CLI);
			HANDLE(RTS);
			HANDLE(ADC);
			HANDLE(ROR);
			HANDLE(PLA);
			HANDLE(BVS);
			HANDLE(SEI);
			HANDLE(STA);
			HANDLE(STY);
			HANDLE(STX);
			HANDLE(DEY);
			HANDLE(TXA);
			HANDLE(BCC);
			HANDLE(TYA);
			HANDLE(TXS);
			HANDLE(LDY);
			HANDLE(LDA);
			HANDLE(LDX);
			HANDLE(TAY);
			HANDLE(TAX);
			HANDLE(BCS);
			HANDLE(CLV);
			HANDLE(TSX);
			HANDLE(CPY);
			HANDLE(CMP);
			HANDLE(DEC);
			HANDLE(INY);
			HANDLE(DEX);
			HANDLE(BNE);
			HANDLE(CLD);
			HANDLE(CPX);
			HANDLE(SBC);
			HANDLE(INC);
			HANDLE(INX);
			HANDLE(NOP);
			HANDLE(BEQ);
			HANDLE(SED);
		default: return "???";
		}
#undef HANDLE
	}

	// observations

	// 3-byte instructions, other than jump instructions, are the last 4 in every 16, and also the 25th element in every 32

}
