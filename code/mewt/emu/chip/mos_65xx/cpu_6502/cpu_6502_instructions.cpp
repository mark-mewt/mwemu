
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"

namespace mewt::emu::chip::mos_65xx::cpu_6502
{

	using op = opcode_t;
	using from = data_loc_t;
	using to = data_loc_t;
	using with = data_loc_t;
	using do_ = operation_t;
	using flag = flag_action_t;

	static instruction_t instructions[] = {
		//Dec	Hex	Opcode		Src	Dest	Ref	Op	Condition	Flags	Bytes	Cycles	Page Straddle Cycle	Branch Taken Cycle	Description
		{ 0x00, op::BRK, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	Break		B	1	7
		{ 0x01, op::ORA, from::IndX, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	2	6
		{ 0x02, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x03, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x04, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x05, op::ORA, from::Zpge, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	2	3
		{ 0x06, op::ASL, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	2	5
		{ 0x07, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x08, op::PHP, from::Stus, to::Stck, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	3
		{ 0x09, op::ORA, from::Imm8, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	2	2
		{ 0x0A, op::ASL, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	1	2
		{ 0x0B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x0C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x0D, op::ORA, from::Ptr_, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	3	4
		{ 0x0E, op::ASL, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	3	6
		{ 0x0F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x10, op::BPL, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	Plus	-	2	2	1	1
		{ 0x11, op::ORA, from::IndY, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	2	5	1
		{ 0x12, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x13, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x14, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x15, op::ORA, from::ZppX, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	2	4
		{ 0x16, op::ASL, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	2	6
		{ 0x17, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x18, op::CLC, from::Flag, to::Flag, with::Bit0, do_::AndN, flag::None }, //					C	1	2			Clear Carry
		{ 0x19, op::ORA, from::PtrY, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	3	4	1
		{ 0x1A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1D, op::ORA, from::PtrX, to::RegA, with::RegA, do_::Or__, flag::Nrml }, //	Or	-	N,Z	3	4	1
		{ 0x1E, op::ASL, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	3	7
		{ 0x1F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x20, op::JSR, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	JSR		?	3	6			Jump to subroutine
		{ 0x21, op::AND, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	AndWithDest		N,Z	2	6
		{ 0x22, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x23, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x24, op::BIT, from::Zpge, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	BitTest		N,V,Z	2	3			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x25, op::AND, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	3
		{ 0x26, op::ROL, from::Zpge, to::Zpge, with::None, do_::Rol_, flag::Nrml }, //	RotateLeft		N,Z,C	2	5
		{ 0x27, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x28, op::PLP, from::Stck, to::Stus, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	4
		{ 0x29, op::AND, from::Imm8, to::RegA, with::RegA, do_::And_, flag::Nrml }, //	Add		N,Z	2	2
		{ 0x2A, op::ROL, from::RegA, to::RegA, with::None, do_::Rol_, flag::Nrml }, //	RotateLeft		N,Z,C	1	2
		{ 0x2B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x2C, op::BIT, from::Ptr_, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	BitTest		N,V,Z	3	4			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x2D, op::AND, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4
		{ 0x2E, op::ROL, from::Ptr_, to::Ptr_, with::None, do_::Rol_, flag::Nrml }, //	RotateLeft		N,Z,C	3	6
		{ 0x2F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x30, op::BMI, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	Minus	-	2	2	1	1
		{ 0x31, op::AND, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	5	1
		{ 0x32, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x33, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x34, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x35, op::AND, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	4
		{ 0x36, op::ROL, from::ZppX, to::ZppX, with::None, do_::Rol_, flag::Nrml }, //	RotateLeft		N,Z,C	2	6
		{ 0x37, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x38, op::SEC, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //					C	1	2			Set Carry
		{ 0x39, op::AND, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4	1
		{ 0x3A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3D, op::AND, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4	1
		{ 0x3E, op::ROL, from::PtrX, to::PtrX, with::None, do_::Rol_, flag::Nrml }, //	RotateLeft		N,Z,C	3	7
		{ 0x3F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x40, op::RTI, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	ReturnInt	-	All	1	6			Return from interrupt
		{ 0x41, op::EOR, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	6
		{ 0x42, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x43, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x44, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x45, op::EOR, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	3
		{ 0x46, op::LSR, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	2	5
		{ 0x47, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x48, op::PHA, from::RegA, to::Stck, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	3
		{ 0x49, op::EOR, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	2
		{ 0x4A, op::LSR, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	1	2
		{ 0x4B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x4C, op::JMP, from::None, to::None, with::None, do_::ToDo, flag::None }, //	Branch	-	-	3	3
		{ 0x4D, op::EOR, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4
		{ 0x4E, op::LSR, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	3	6
		{ 0x4F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x50, op::BVC, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	Overflow Clear	-	2	2	1	1
		{ 0x51, op::EOR, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	5	1
		{ 0x52, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x53, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x54, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x55, op::EOR, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	4
		{ 0x56, op::LSR, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	2	6
		{ 0x57, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x58, op::CLI, from::Flag, to::Flag, with::Bit2, do_::AndN, flag::None }, //					I	1	2			Clear Interrupt
		{ 0x59, op::EOR, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4	1
		{ 0x5A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5D, op::EOR, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4	1
		{ 0x5E, op::LSR, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	3	7
		{ 0x5F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x60, op::RTS, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	ReturnSub	-	-	1	6			Return from Subroutine
		{ 0x61, op::ADC, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	6
		{ 0x62, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x63, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x64, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x65, op::ADC, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	3
		{ 0x66, op::ROR, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	2	5
		{ 0x67, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x68, op::PLA, from::Stck, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	4
		{ 0x69, op::ADC, from::Imm8, to::RegA, with::RegA, do_::Add_, flag::Nrml }, //	Add2DstC		N,V,Z,C	2	2
		{ 0x6A, op::ROR, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	1	2
		{ 0x6B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x6C, op::JMP, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //		Branch	-	-	3	5
		{ 0x6D, op::ADC, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4
		{ 0x6E, op::ROR, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	3	6
		{ 0x6F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x70, op::BVS, from::Imm8, to::ToDo, with::None, do_::ToDo, flag::None }, //	Branch	OverflowSet	-	2	2	1	1
		{ 0x71, op::ADC, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	5	1
		{ 0x72, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x73, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x74, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x75, op::ADC, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	4
		{ 0x76, op::ROR, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	2	6
		{ 0x77, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x78, op::SEI, from::Flag, to::Flag, with::Bit2, do_::Or__, flag::None }, //					I	1	2			Set Interrupt
		{ 0x79, op::ADC, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4	1
		{ 0x7A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7D, op::ADC, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4	1
		{ 0x7E, op::ROR, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	3	7
		{ 0x7F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x80, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x81, op::STA, from::RegA, to::IndX, with::None, do_::Copy, flag::None }, //	-		-	2	6
		{ 0x82, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x83, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x84, op::STY, from::RegY, to::Zpge, with::None, do_::Copy, flag::None }, //	-	-	-	2	3
		{ 0x85, op::STA, from::RegA, to::Zpge, with::None, do_::Copy, flag::None }, //	-		-	2	3
		{ 0x86, op::STX, from::RegX, to::Zpge, with::None, do_::Copy, flag::None }, //	-	-	-	2	3
		{ 0x87, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x88, op::DEY, from::RegY, to::RegY, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	1	2
		{ 0x89, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x8A, op::TXA, from::RegX, to::RegA, with::None, do_::Copy, flag::Nrml }, //			N,Z	1	2
		{ 0x8B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x8C, op::STY, from::RegY, to::Ptr_, with::None, do_::Copy, flag::None }, //	-	-	-	3	4
		{ 0x8D, op::STA, from::RegA, to::Ptr_, with::None, do_::Copy, flag::None }, //	-		-	3	4
		{ 0x8E, op::STX, from::RegX, to::Ptr_, with::None, do_::Copy, flag::None }, //	-	-	-	3	4
		{ 0x8F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x90, op::BCC, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	CarryClear	-	2	2	1	1
		{ 0x91, op::STA, from::RegA, to::IndY, with::None, do_::Copy, flag::None }, //	-		-	2	6
		{ 0x92, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x93, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x94, op::STY, from::RegY, to::ZppX, with::None, do_::Copy, flag::None }, //	-	-	-	2	4
		{ 0x95, op::STA, from::RegA, to::ZppX, with::None, do_::Copy, flag::None }, //	-		-	2	4
		{ 0x96, op::STX, from::RegX, to::ZppY, with::None, do_::Copy, flag::None }, //	-	-	-	2	4
		{ 0x97, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x98, op::TYA, from::RegY, to::RegA, with::None, do_::Copy, flag::Nrml }, //			N,Z	1	2
		{ 0x99, op::STA, from::RegA, to::PtrY, with::None, do_::Copy, flag::None }, //	-		-	3	5
		{ 0x9A, op::TXS, from::RegX, to::RegS, with::None, do_::Copy, flag::None }, //	-	-	-	1	2
		{ 0x9B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9D, op::STA, from::RegA, to::PtrX, with::None, do_::Copy, flag::None }, //	-		-	3	5
		{ 0x9E, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA0, op::LDY, from::Imm8, to::RegY, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	2			Load Y Immediate
		{ 0xA1, op::LDA, from::IndX, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	6
		{ 0xA2, op::LDX, from::Imm8, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	2			Load X Immediate
		{ 0xA3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA4, op::LDY, from::Zpge, to::RegY, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	3
		{ 0xA5, op::LDA, from::Zpge, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	3
		{ 0xA6, op::LDX, from::Zpge, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	3
		{ 0xA7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA8, op::TAY, from::RegA, to::RegY, with::None, do_::Copy, flag::Nrml }, //			N,Z	1	2
		{ 0xA9, op::LDA, from::Imm8, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	2			Load Accumulator Immediate
		{ 0xAA, op::TAX, from::RegA, to::RegX, with::None, do_::Copy, flag::Nrml }, //			N,Z	1	2
		{ 0xAB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xAC, op::LDY, from::Ptr_, to::RegY, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4
		{ 0xAD, op::LDA, from::Ptr_, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4
		{ 0xAE, op::LDX, from::Ptr_, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4
		{ 0xAF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB0, op::BCS, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	CarrySet	-	2	2	1	1
		{ 0xB1, op::LDA, from::IndY, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	5	1
		{ 0xB2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB4, op::LDY, from::ZppX, to::RegY, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	4
		{ 0xB5, op::LDA, from::ZppX, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	4
		{ 0xB6, op::LDX, from::ZppY, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	4
		{ 0xB7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB8, op::CLV, from::Flag, to::Flag, with::Bit6, do_::AndN, flag::None }, //					O	1	2			Clear Overflow
		{ 0xB9, op::LDA, from::PtrY, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4	1
		{ 0xBA, op::TSX, from::RegS, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-	-	-	1	2
		{ 0xBB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xBC, op::LDY, from::PtrX, to::RegY, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4	1
		{ 0xBD, op::LDA, from::PtrX, to::RegA, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4	1
		{ 0xBE, op::LDX, from::PtrY, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	3	4	1
		{ 0xBF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC0, op::CPY, from::Imm8, to::None, with::RegY, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	2
		{ 0xC1, op::CMP, from::IndX, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	6
		{ 0xC2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC4, op::CPY, from::Zpge, to::None, with::RegY, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	3
		{ 0xC5, op::CMP, from::Zpge, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	3
		{ 0xC6, op::DEC, from::Zpge, to::Zpge, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	2	5
		{ 0xC7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC8, op::INY, from::RegY, to::RegY, with::Bit0, do_::Inc_, flag::Nrml }, //	Inc		N,Z	1	2
		{ 0xC9, op::CMP, from::Imm8, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	2
		{ 0xCA, op::DEX, from::RegX, to::RegX, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	1	2
		{ 0xCB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xCC, op::CPY, from::Ptr_, to::None, with::RegY, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	3	4
		{ 0xCD, op::CMP, from::Ptr_, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	3	4
		{ 0xCE, op::DEC, from::Ptr_, to::Ptr_, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	3	6
		{ 0xCF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD0, op::BNE, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	NotEqual	-	2	2	1	1
		{ 0xD1, op::CMP, from::IndY, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	5	1
		{ 0xD2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD4, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD5, op::CMP, from::ZppX, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	4
		{ 0xD6, op::DEC, from::ZppX, to::ZppX, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	2	6
		{ 0xD7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD8, op::CLD, from::Flag, to::Flag, with::Bit3, do_::AndN, flag::None }, //					D	1	2			Clear Decimal
		{ 0xD9, op::CMP, from::PtrY, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	3	4	1
		{ 0xDA, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDC, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDD, op::CMP, from::PtrX, to::None, with::RegA, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	3	4	1
		{ 0xDE, op::DEC, from::PtrX, to::PtrX, with::Bit0, do_::Dec_, flag::Nrml }, //	Dec		N,Z	3	7
		{ 0xDF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE0, op::CPX, from::Imm8, to::None, with::RegX, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	2
		{ 0xE1, op::SBC, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	6
		{ 0xE2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE4, op::CPX, from::Zpge, to::None, with::RegX, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	2	3
		{ 0xE5, op::SBC, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	3
		{ 0xE6, op::INC, from::Zpge, to::Zpge, with::Bit0, do_::Inc_, flag::Nrml }, //	Inc		N,Z	2	5
		{ 0xE7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE8, op::INX, from::RegX, to::RegX, with::Bit0, do_::Inc_, flag::Nrml }, //	Inc		N,Z	1	2
		{ 0xE9, op::SBC, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	2
		{ 0xEA, op::NOP, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //			-	-	-	1	2
		{ 0xEB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xEC, op::CPX, from::Ptr_, to::None, with::RegX, do_::Sub_, flag::Nrml }, //	Compare		N,Z,C	3	4
		{ 0xED, op::SBC, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4
		{ 0xEE, op::INC, from::Ptr_, to::Ptr_, with::Bit0, do_::Inc_, flag::Nrml }, //	Inc		N,Z	3	6
		{ 0xEF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF0, op::BEQ, from::Imm8, to::None, with::None, do_::ToDo, flag::None }, //	Branch	Equal	-	2	2	1	1
		{ 0xF1, op::SBC, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	5	1
		{ 0xF2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF4, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF5, op::SBC, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	4
		{ 0xF6, op::INC, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	Inc		N,Z	2	6
		{ 0xF7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF8, op::SED, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //					D	1	2			Set Decimal
		{ 0xF9, op::SBC, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4	1
		{ 0xFA, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFC, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFD, op::SBC, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4	1
		{ 0xFE, op::INC, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	Inc		N,Z	3	7
		{ 0xFF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
	};

	/*

		group A = (x & 0x1f) == 0x10 => branch instructions
		{ 0x10, op::BPL, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	Plus	-	2	2	1	1
		{ 0x30, op::BMI, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	Minus	-	2	2	1	1
		{ 0x50, op::BVC, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	Overflow Clear	-	2	2	1	1
		{ 0x70, op::BVS, from::Imm8, to::ToDo, with::None, do_::ToDo, flag::ToDo }, //	Branch	OverflowSet	-	2	2	1	1
		{ 0x90, op::BCC, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	CarryClear	-	2	2	1	1
		{ 0xB0, op::BCS, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	CarrySet	-	2	2	1	1
		{ 0xD0, op::BNE, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	NotEqual	-	2	2	1	1
		{ 0xF0, op::BEQ, from::Imm8, to::None, with::None, do_::ToDo, flag::ToDo }, //	Branch	Equal	-	2	2	1	1

		group B = (x & 0x9f) == 0 => call instructions
		{ 0x00, op::BRK, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	Break		B	1	7
		{ 0x20, op::JSR, from::Im16, to::ToDo, with::None, do_::ToDo, flag::ToDo }, //	JSR		?	3	6			Jump to subroutine
		{ 0x40, op::RTI, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	ReturnInt	-	All	1	6			Return from interrupt
		{ 0x60, op::RTS, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //	ReturnSub	-	-	1	6			Return from Subroutine


		group Z = everything else
		-----
		{ 0x01, op::ORA, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	2	6
		{ 0x02, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x03, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x04, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x05, op::ORA, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	2	3
		{ 0x06, op::ASL, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	2	5
		{ 0x07, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x08, op::PHP, from::Stus, to::Stck, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	3
		{ 0x09, op::ORA, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	2	2
		{ 0x0A, op::ASL, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	1	2
		{ 0x0B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x0C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x0D, op::ORA, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	3	4
		{ 0x0E, op::ASL, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	3	6
		{ 0x0F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x11, op::ORA, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	2	5	1
		{ 0x12, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x13, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x14, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x15, op::ORA, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	2	4
		{ 0x16, op::ASL, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	2	6
		{ 0x17, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x18, op::CLC, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //					C	1	2			Clear Carry
		{ 0x19, op::ORA, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	3	4	1
		{ 0x1A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x1D, op::ORA, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Or	-	N,Z	3	4	1
		{ 0x1E, op::ASL, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	ASL1		N,Z,C	3	7
		{ 0x1F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x21, op::AND, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	AndWithDest		N,Z	2	6
		{ 0x22, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x23, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x24, op::BIT, from::Zpge, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	BitTest		N,V,Z	2	3			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x25, op::AND, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	3
		{ 0x26, op::ROL, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	RotateLeft		N,Z,C	2	5
		{ 0x27, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x28, op::PLP, from::Stck, to::Stus, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	4
		{ 0x29, op::AND, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	2
		{ 0x2A, op::ROL, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	RotateLeft		N,Z,C	1	2
		{ 0x2B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x2C, op::BIT, from::Ptr_, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	BitTest		N,V,Z	3	4			https://www.pagetable.com/c64ref/6502/?tab=2#BIT
		{ 0x2D, op::AND, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4
		{ 0x2E, op::ROL, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	RotateLeft		N,Z,C	3	6
		{ 0x2F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x31, op::AND, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	5	1
		{ 0x32, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x33, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x34, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x35, op::AND, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	2	4
		{ 0x36, op::ROL, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	RotateLeft		N,Z,C	2	6
		{ 0x37, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x38, op::SEC, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //					C	1	2			Set Carry
		{ 0x39, op::AND, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4	1
		{ 0x3A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x3D, op::AND, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add		N,Z	3	4	1
		{ 0x3E, op::ROL, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	RotateLeft		N,Z,C	3	7
		{ 0x3F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x41, op::EOR, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	6
		{ 0x42, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x43, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x44, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x45, op::EOR, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	3
		{ 0x46, op::LSR, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	2	5
		{ 0x47, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x48, op::PHA, from::RegA, to::Stck, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	3
		{ 0x49, op::EOR, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	2
		{ 0x4A, op::LSR, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	1	2
		{ 0x4B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x4C, op::JMP, from::Im16, to::ToDo, with::None, do_::ToDo, flag::ToDo }, //	Branch	-	-	3	3
		{ 0x4D, op::EOR, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4
		{ 0x4E, op::LSR, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	3	6
		{ 0x4F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x51, op::EOR, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	5	1
		{ 0x52, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x53, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x54, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x55, op::EOR, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	2	4
		{ 0x56, op::LSR, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	2	6
		{ 0x57, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x58, op::CLI, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //					I	1	2			Clear Interrupt
		{ 0x59, op::EOR, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4	1
		{ 0x5A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x5D, op::EOR, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Xor		N,Z	3	4	1
		{ 0x5E, op::LSR, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	Lsr	-	N,Z,C	3	7
		{ 0x5F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x61, op::ADC, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	6
		{ 0x62, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x63, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x64, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x65, op::ADC, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	3
		{ 0x66, op::ROR, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	2	5
		{ 0x67, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x68, op::PLA, from::Stck, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	1	4
		{ 0x69, op::ADC, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	2
		{ 0x6A, op::ROR, from::RegA, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	1	2
		{ 0x6B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x6C, op::JMP, from::Ptr_, to::ToDo, with::None, do_::ToDo, flag::ToDo }, //		Branch	-	-	3	5
		{ 0x6D, op::ADC, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4
		{ 0x6E, op::ROR, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	3	6
		{ 0x6F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x71, op::ADC, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	5	1
		{ 0x72, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x73, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x74, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x75, op::ADC, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	2	4
		{ 0x76, op::ROR, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	2	6
		{ 0x77, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x78, op::SEI, from::Flag, to::Flag, with::Bit2, do_::Or__, flag::None }, //					I	1	2			Set Interrupt
		{ 0x79, op::ADC, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4	1
		{ 0x7A, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x7D, op::ADC, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	Add2DstC		N,V,Z,C	3	4	1
		{ 0x7E, op::ROR, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	RotateRight		N,Z,C	3	7
		{ 0x7F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x80, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x81, op::STA, from::RegA, to::IndX, with::None, do_::ToDo, flag::ToDo }, //	-		-	2	6
		{ 0x82, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x83, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x84, op::STY, from::RegY, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	2	3
		{ 0x85, op::STA, from::RegA, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	-		-	2	3
		{ 0x86, op::STX, from::RegX, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	2	3
		{ 0x87, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x88, op::DEY, from::RegY, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	Dec		N,Z	1	2
		{ 0x89, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x8A, op::TXA, from::RegX, to::RegA, with::None, do_::ToDo, flag::ToDo }, //			N,Z	1	2
		{ 0x8B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x8C, op::STY, from::RegY, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	3	4
		{ 0x8D, op::STA, from::RegA, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	-		-	3	4
		{ 0x8E, op::STX, from::RegX, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	3	4
		{ 0x8F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0x91, op::STA, from::RegA, to::IndY, with::None, do_::ToDo, flag::ToDo }, //	-		-	2	6
		{ 0x92, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x93, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x94, op::STY, from::RegY, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	2	4
		{ 0x95, op::STA, from::RegA, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	-		-	2	4
		{ 0x96, op::STX, from::RegX, to::ZppY, with::None, do_::ToDo, flag::ToDo }, //	-	-	-	2	4
		{ 0x97, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x98, op::TYA, from::RegY, to::RegA, with::None, do_::ToDo, flag::ToDo }, //			N,Z	1	2
		{ 0x99, op::STA, from::RegA, to::PtrY, with::None, do_::ToDo, flag::ToDo }, //	-		-	3	5
		{ 0x9A, op::TXS, from::RegX, to::RegS, with::None, do_::Copy, flag::None }, //	-	-	-	1	2
		{ 0x9B, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9C, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9D, op::STA, from::RegA, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	-		-	3	5
		{ 0x9E, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0x9F, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA0, op::LDY, from::Imm8, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	2			Load Y Immediate
		{ 0xA1, op::LDA, from::IndX, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	6
		{ 0xA2, op::LDX, from::Imm8, to::RegX, with::None, do_::Copy, flag::Nrml }, //	-		N,Z	2	2			Load X Immediate
		{ 0xA3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA4, op::LDY, from::Zpge, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	3
		{ 0xA5, op::LDA, from::Zpge, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	3
		{ 0xA6, op::LDX, from::Zpge, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	3
		{ 0xA7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xA8, op::TAY, from::RegA, to::RegY, with::None, do_::ToDo, flag::ToDo }, //			N,Z	1	2
		{ 0xA9, op::LDA, from::Imm8, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-		N,Z	2	2			Load Accumulator Immediate
		{ 0xAA, op::TAX, from::RegA, to::RegX, with::None, do_::ToDo, flag::ToDo }, //			N,Z	1	2
		{ 0xAB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xAC, op::LDY, from::Ptr_, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4
		{ 0xAD, op::LDA, from::Ptr_, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4
		{ 0xAE, op::LDX, from::Ptr_, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4
		{ 0xAF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0xB1, op::LDA, from::IndY, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	2	5	1
		{ 0xB2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB4, op::LDY, from::ZppX, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	2	4
		{ 0xB5, op::LDA, from::ZppX, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	2	4
		{ 0xB6, op::LDX, from::ZppY, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	2	4
		{ 0xB7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xB8, op::CLV, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //							O	1	2			Clear Overflow
		{ 0xB9, op::LDA, from::PtrY, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4	1
		{ 0xBA, op::TSX, from::RegS, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	-	-					-	1	2
		{ 0xBB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xBC, op::LDY, from::PtrX, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4	1
		{ 0xBD, op::LDA, from::PtrX, to::RegA, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4	1
		{ 0xBE, op::LDX, from::PtrY, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	-						N,Z	3	4	1
		{ 0xBF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC0, op::CPY, from::Imm8, to::None, with::RegY, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	2
		{ 0xC1, op::CMP, from::IndX, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	6
		{ 0xC2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC4, op::CPY, from::Zpge, to::None, with::RegY, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	3
		{ 0xC5, op::CMP, from::Zpge, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	3
		{ 0xC6, op::DEC, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	Dec					N,Z	2	5
		{ 0xC7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xC8, op::INY, from::RegY, to::RegY, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	1	2
		{ 0xC9, op::CMP, from::Imm8, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	2
		{ 0xCA, op::DEX, from::RegX, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	Dec					N,Z	1	2
		{ 0xCB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xCC, op::CPY, from::Ptr_, to::None, with::RegY, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	3	4
		{ 0xCD, op::CMP, from::Ptr_, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	3	4
		{ 0xCE, op::DEC, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	Dec					N,Z	3	6
		{ 0xCF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0xD1, op::CMP, from::IndY, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	5	1
		{ 0xD2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD4, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD5, op::CMP, from::ZppX, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	4
		{ 0xD6, op::DEC, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	Dec					N,Z	2	6
		{ 0xD7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xD8, op::CLD, from::Flag, to::Flag, with::Bit3, do_::AndN, flag::None }, //							D	1	2			Clear Decimal
		{ 0xD9, op::CMP, from::PtrY, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	3	4	1
		{ 0xDA, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDC, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xDD, op::CMP, from::PtrX, to::None, with::RegA, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	3	4	1
		{ 0xDE, op::DEC, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	Dec					N,Z	3	7
		{ 0xDF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE0, op::CPX, from::Imm8, to::None, with::RegX, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	2
		{ 0xE1, op::SBC, from::IndX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	6
		{ 0xE2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE4, op::CPX, from::Zpge, to::None, with::RegX, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	2	3
		{ 0xE5, op::SBC, from::Zpge, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	3
		{ 0xE6, op::INC, from::Zpge, to::Zpge, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	2	5
		{ 0xE7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xE8, op::INX, from::RegX, to::RegX, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	1	2
		{ 0xE9, op::SBC, from::Imm8, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	2
		{ 0xEA, op::NOP, from::None, to::None, with::None, do_::ToDo, flag::ToDo }, //			-	-			-	1	2
		{ 0xEB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xEC, op::CPX, from::Ptr_, to::None, with::RegX, do_::ToDo, flag::ToDo }, //	Compare				N,Z,C	3	4
		{ 0xED, op::SBC, from::Ptr_, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4
		{ 0xEE, op::INC, from::Ptr_, to::Ptr_, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	3	6
		{ 0xEF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		-----
		{ 0xF1, op::SBC, from::IndY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	5	1
		{ 0xF2, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF3, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF4, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF5, op::SBC, from::ZppX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	2	4
		{ 0xF6, op::INC, from::ZppX, to::ZppX, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	2	6
		{ 0xF7, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xF8, op::SED, from::ToDo, to::ToDo, with::ToDo, do_::ToDo, flag::ToDo }, //							D	1	2			Set Decimal
		{ 0xF9, op::SBC, from::PtrY, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4	1
		{ 0xFA, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFB, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFC, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //
		{ 0xFD, op::SBC, from::PtrX, to::RegA, with::RegA, do_::ToDo, flag::ToDo }, //	SubWithCarry	-	N,V,Z,C	3	4	1
		{ 0xFE, op::INC, from::PtrX, to::PtrX, with::None, do_::ToDo, flag::ToDo }, //	Inc					N,Z	3	7
		{ 0xFF, op::___, from::____, to::____, with::____, do_::ToDo, flag::ToDo }, //


	*/

	instruction_t* get_instructions()
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
