
#include "mewt/emu/chip/mos_65xx/sid_6581/sid_6581.h"
#include "mewt/diag/log.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/chip/clock/clock.h"

#include <span>

namespace mewt::emu::chip::mos_65xx
{

	// #mwToDo: Move elsewhere
	constexpr types::Byte kSidMaxReadableRegister = types::fromUnderlyingType<types::Byte>(0x1d);
	constexpr types::Byte kSidMaxWritableRegister = types::fromUnderlyingType<types::Byte>(0x18);

	Data sid_6581_t::io_controller_t::read(Address address)
	{
		// https://www.c64-wiki.com/wiki/SID
		// address = toWord(lowByte(address)); // regs mirrored every 0x100
		// address &= 0xff;
		auto local_address = lowByte(address); // regs mirrored every 0x100
		if (local_address > kSidMaxReadableRegister)
			return types::zero;
		return types::ByteSpan(_regs)[lowBits<5>(local_address)];
		// return *((Data*)&_regs + address);
	}

	void sid_6581_t::io_controller_t::write(Address address, Data data)
	{
		auto local_address = lowByte(address); // regs mirrored every 0x100
		// address = toWord(lowByte(address));
		//  address &= 0xff;
		if (local_address > kSidMaxWritableRegister)
			return;
		auto memory = types::ByteSpan(_regs);
		memory[lowBits<5>(local_address)] = data;
		//*((Data*)&_regs + address) = data;
	}

	sid_6581_t::sid_6581_t(const clock_source_t& clock)
		 : _clock(clock)
	{
		// logger().log("sid_6581_t::construct");
	}
	async::Future<> sid_6581_t::read_mem()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 2);
	}
	async::Future<> sid_6581_t::run_inst()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await read_mem();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 2);
		co_await _clock.nextTick();
		// logger().log("%s: %d", __FUNCTION__, 3);
	}
	async::Future<> sid_6581_t::run_gpu()
	{
		// logger().log("%s: %d", __FUNCTION__, 0);
		co_await run_inst();
		// logger().log("%s: %d", __FUNCTION__, 1);
		co_await run_inst();
		// logger().log("%s: %d", __FUNCTION__, 2);
	}

}
