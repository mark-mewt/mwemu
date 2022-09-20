
#include "mewt/emu/chip/mos_65xx/cia_6526/cia_6526.h"
#include "mewt/diag/log.h"

#include <span>

namespace mewt::emu::chip::mos_65xx
{

	template <typename TType>
		requires(std::is_pod_v<TType>)
	constexpr auto asBytes(TType& type)
	{
		return std::as_writable_bytes(std::span(std::addressof(type), 1));
	}

	// https://www.c64-wiki.com/wiki/CIA

	constexpr Address kCIAAddressMask = sizeof(cia_6526_t::regs_t) - 1;

	auto cia_6526_t::read(Address address)
		 -> Data
	{
		return static_cast<Data>(asBytes(_regs)[address & kCIAAddressMask]);
	}

	void cia_6526_t::write(Address address, Data data)
	{
		asBytes(_regs)[address & kCIAAddressMask] = static_cast<std::byte>(data);
	}

}
