
#include "mewt/emu/chip/mos_65xx/cia_6526/cia_6526.h"
#include "mewt/diag/log.h"

#include <span>

namespace mewt::emu::chip::mos_65xx
{

	// https://www.c64-wiki.com/wiki/CIA

	// constexpr Address kCIAAddressMask = types::fromUnderlyingType<Address>(sizeof(cia_6526_t::regs_t) - 1);

	auto cia_6526_t::read(Address address)
		 -> Data
	{
		return types::ByteSpan(_regs)[lowNibble(address)];
		// return static_cast<Data>(asBytes(_regs)[address & kCIAAddressMask]);
	}

	void cia_6526_t::write(Address address, Data data)
	{
		(types::ByteSpan(_regs))[lowNibble(address)] = data;
		// asBytes(_regs)[address & kCIAAddressMask] = static_cast<std::byte>(data);
	}

}
