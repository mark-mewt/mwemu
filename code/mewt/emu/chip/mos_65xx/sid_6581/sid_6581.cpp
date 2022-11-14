
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"

#include "mewt/async/future_promise.h"
#include "mewt/diag/log.h"
#include "mewt/emu/chip/clock/clock.h"
#include "mewt/types/byte_span.h"

#include <span>

namespace mewt::emu::chip::mos_65xx
{

	// #mwToDo: Move elsewhere
	///constexpr SID::Address kSidMaxReadableRegister { 0x1d };
	///constexpr SID::Address kSidMaxWritableRegister { 0x18 };

	/*auto SID::IOController::read(Address address) -> Data
	{
		// https://www.c64-wiki.com/wiki/SID
		// address = toWord(lowByte(address)); // regs mirrored every 0x100
		// address &= 0xff;
		auto local_address = address;
		// lowByte(address); // regs mirrored every 0x100
		if (local_address > kSidMaxReadableRegister)
			return types::Byte { 0 };
		// types::zero;
		using BS = decltype(types::ByteSpan(_regs));
		using Addr = typename BS::Address;
		static_assert(sizeof(Addr) == sizeof(Address));
		return types::ByteSpan(_regs)[ (local_address)];
		// return *((Data*)&_regs + address);
	}

	void SID::IOController::write(Address address, Data data)
	{
		auto local_address = address;
		// lowByte(address); // regs mirrored every 0x100
		//  address = toWord(lowByte(address));
		//   address &= 0xff;
		if (local_address > kSidMaxWritableRegister)
			return;
		auto memory = types::ByteSpan(_regs);
		memory[(local_address)] = data;
		/((Data*)&_regs + address) = data;
	}*/

	SID::SID(const clock_source_t& clock)
		 : _clock(clock)
	{
		// logger().log("sid_6581_t::construct");
	}

	async::Future<> SID::readMem()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 2);
	}

	async::Future<> SID::runInst()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await readMem();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 2);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 3);
	}

	async::Future<> SID::runGPU()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await runInst();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await runInst();
		// logger().log("%s: %d", __FUNCTION__, 2);
	}

}
