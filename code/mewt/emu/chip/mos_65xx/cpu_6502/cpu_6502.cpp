
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502.h"
#include "mewt/types/intrusive_stack.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/diag/log.h"
#include "mewt/emu/chip/mos_65xx/cpu_6502/cpu_6502_instructions.h"

/*
* 
* ToDo:
* 
* - Write a splitter for clock cycles, so we can run things on sub-cycles.
* - Can we separate the list of awaiters from the clock source (and remove the mutable)?
* - Get C64 rom and start emulating it
* - Start emulating the Vic-2
* - Enable all warnings in compiler - need to shift out the win32 stuff first
* - Make address sanitizer work
* 
*/

namespace mewt::emu::chip::mos_65xx
{

   // todo: move these elsewhere
   constexpr std::uint8_t highbyte(uint16_t x) { return x >> 8; }
   constexpr std::uint8_t lowbyte(uint16_t x) { return x & 0xff; }
   constexpr std::uint16_t makeword(uint8_t low, uint8_t high) { return ((uint16_t)high << 8) | low; }

   cpu_6502_t::cpu_6502_t(const clock_source_t& clock, memory_interface_t& memory_interface)
      : _clock(clock), _memory_interface(memory_interface)
   {
      logger().log("%s: %d", __FUNCTION__, 0);
   }

   async::future<data_t> cpu_6502_t::read_data(address_t address)
   {
      co_await _clock.next_tick();
      auto data = _memory_interface.read(address);
      //logger().log("[0x%04X] -> 0x%02X", address, data);
      co_return data;
   }

   async::future<address_t> cpu_6502_t::read_address(address_t address)
   {
      //logger().log("%s: %d", __FUNCTION__, 0);
      auto low = co_await read_data(address);
      auto high = co_await read_data(address + 1);
      co_return (address_t)low | ((address_t)high << 8);
   }

   async::future<address_t> cpu_6502_t::read_address_zp(data_t offset)
   {
      //logger().log("%s: %d", __FUNCTION__, 0);
      auto low = co_await read_data(offset);
      auto high = co_await read_data((offset + 1) & 0xff);
      co_return (address_t)low | ((address_t)high << 8);
   }

   async::future<> cpu_6502_t::write_data(address_t address, data_t data)
   {
      //logger().log("[0x%04X] <- 0x%02X", address, data);
      co_await _clock.next_tick();
      co_return _memory_interface.write(address, data);
   }

	class no_reentry_check_t {
		static bool _is_in;
	public:
		no_reentry_check_t() {
			if (_is_in)
				throw std::exception("previous instruction not ended");
			_is_in = true;
		}
		~no_reentry_check_t() noexcept(false) {
			if (!_is_in)
				throw std::exception("instruction not started");
			_is_in = false;
		}
	};

	bool no_reentry_check_t::_is_in = false;

   async::future<> cpu_6502_t::run_inst()
   {
		no_reentry_check_t no_reentry_check;
		using namespace cpu_6502;
      //logger().log("--------------------------");
      auto instCode = co_await read_data(_pc);
		auto& inst = get_instructions()[instCode];
      auto immLow = co_await read_data(_pc+1);
      data_t immHigh = 0;
      address_t immAddr = 0;
      if (inst.is_3_byte())
      {
         immHigh = co_await read_data(_pc + 2);
         immAddr = makeword(immLow, immHigh);
         //logger().log("0x%04X: %02X %s(%04X)", _pc, instCode, to_string(inst.opcode), immAddr);
		}
		else {
			//logger().log("0x%04X: %02X %s(%02X)", _pc, instCode, to_string(inst.opcode), immLow);
		}
      ++_pc;
      if (!inst.is_1_byte())
      {
         ++_pc;
         if(inst.is_3_byte())
            ++_pc;
      }
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
      data_t val = 0;
      switch (inst.src)
      {
		case data_loc_t::Imm8:
         val = immLow;
         break;
		case data_loc_t::Flag:
         val = _reg_flags;
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
      default:
         throw std::exception("implement");
      }
      data_t ref = 0;
      switch (inst.ref)
      {
      case data_loc_t::None:
         break;
      case data_loc_t::Flag:
         ref = _reg_flags;
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
			if (inst.dest != data_loc_t::None) {
				val -= (carry_flag ? 0 : 1);
				bool signs_changed = (val & 0x80) == (ref & 0x80);
				overflow_flag = signs_equal && signs_changed;
			}
			carry_flag = (val & 0x80) == 0;
		} break;
		case operation_t::Add_: {
			bool signs_equal = (val & 0x80) == (ref & 0x80);
			uint16_t val16 = (uint16_t)ref + (uint16_t)val;
			val = (data_t)val16;
			if (inst.dest != data_loc_t::None) {
				val += (carry_flag ? 1 : 0);
				bool signs_changed = (val & 0x80) == (ref & 0x80);
				overflow_flag = signs_equal && signs_changed;
			}
			if (_reg_flags[flag_t::Decimal])
				carry_flag = (val16 > 99);
			else
				carry_flag = (val16 & 0xff00) != 0;
		} break;
		case operation_t::Dec_:
         val -= ref;
         break;
      case operation_t::Inc_:
         val += ref;
         break;
      case operation_t::Rol_:
      {
         auto old_carry = carry_flag;
         carry_flag = (val & 0x80) != 0;
         val = (val << 1) | (old_carry ? 1 : 0);
      }
         break;
      default:
         throw std::exception("implement");
      }
      switch (inst.flag)
      {
      case flag_action_t::None:
         break;
      case flag_action_t::Nrml:
         _reg_flags[flag_t::Zero] = (val == 0);          // set zero flag if value is zero
         _reg_flags[flag_t::Negative] = (val & 0x80) != 0;   // set negative flag if value is negative
         _reg_flags[flag_t::Carry] = carry_flag;
         _reg_flags[flag_t::Overflow] = overflow_flag;
         break;
      default:
         throw std::exception("implement");
      }
      switch (inst.dest)
      {
      case data_loc_t::None:
         break;
      case data_loc_t::Flag:
         _reg_flags = val;
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
      default:
         throw std::exception("implement");
      }
   }

   async::future<> cpu_6502_t::run_cpu()
   {
      _pc = co_await read_address(0xfffc);
      logger().log("%s: %d", __FUNCTION__, 0);
      for (;;)
      {
         co_await run_inst();
      }
   }

   async::future<> cpu_6502_t::handle_branch(cpu_6502::branch_instruction_t inst, data_t immLow)
   {
      bool take_branch = false;
      switch (inst)
      {
		case cpu_6502::branch_instruction_t::BNE:
         take_branch = !_reg_flags[flag_t::Zero];
         break;
		case cpu_6502::branch_instruction_t::BEQ:
			take_branch = _reg_flags[flag_t::Zero];
			break;
		case cpu_6502::branch_instruction_t::BCS:
			take_branch = _reg_flags[flag_t::Carry];
			break;
		case cpu_6502::branch_instruction_t::BCC:
			take_branch = !_reg_flags[flag_t::Carry];
			break;
		case cpu_6502::branch_instruction_t::BMI:
			take_branch = _reg_flags[flag_t::Negative];
			break;
		case cpu_6502::branch_instruction_t::BPL:
			take_branch = !_reg_flags[flag_t::Negative];
			break;
		default:
         throw std::exception("implement");
      }
      if (take_branch)
      {
         co_await _clock.next_tick();
         auto page = _pc & 0xf0;
         _pc += (int8_t)immLow;
         auto new_page = _pc & 0xf0;
         if(new_page != page)
            co_await _clock.next_tick();
      }
      else {
         co_return;
      }
   }


   async::future<> cpu_6502_t::handle_call(cpu_6502::call_instruction_t inst, data_t immLow, data_t immHigh)
   {
      switch (inst)
      {
		case cpu_6502::call_instruction_t::JSR:
      {
         --_pc;
         co_await push(highbyte(_pc));
         co_await push(lowbyte(_pc));
         _pc = makeword(immLow, immHigh);
      }
         break;
		case cpu_6502::call_instruction_t::RTS:
      {
         auto low = co_await pop();
         auto high = co_await pop();
         _pc = makeword(low, high) + 1;
      }
         break;
      default:
         throw std::exception("implement");
      }
      co_return;
   }

   async::future<> cpu_6502_t::handle_jump(cpu_6502::jump_instruction_t inst, address_t immAddr)
   {
      switch (inst)
      {
		case cpu_6502::jump_instruction_t::JMP_Absolute:
         _pc = immAddr;
      break;
		case cpu_6502::jump_instruction_t::JMP_Indirect:
         _pc = co_await read_address(immAddr);
      break;
      default:
         throw std::exception("implement");
      }
   }

   async::future<> cpu_6502_t::push(data_t data)
   {
      co_await write_data(0x100 + _reg_s--, data);
   }

   async::future<data_t> cpu_6502_t::pop()
   {
      co_return co_await read_data(0x100 + ++_reg_s);
   }

   // for accurate cycle events per instruction: https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf

}
