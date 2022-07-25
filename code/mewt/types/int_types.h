
#pragma once

#include <cstdint>

namespace mewt::types
{

	template<int _BitWidth>
	struct integer_info;

	template<int _BitWidth>
	using uint = typename integer_info<_BitWidth>::uint;

	template<>
	struct integer_info<8>
	{
		using uint = std::uint8_t;
	};

	template<>
	struct integer_info<16>
	{
		using uint = std::uint16_t;
	};

	template<>
	struct integer_info<32>
	{
		using uint = std::uint32_t;
	};

	template<>
	struct integer_info<64>
	{
		using uint = std::uint64_t;
	};

}
