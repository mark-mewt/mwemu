
#pragma once

#include <cstdint>

namespace mewt::types
{

	template <int NBitWidth>
	struct integer_info;

	template <int NBitWidth>
	using UInt = typename integer_info<NBitWidth>::UInt;

	template <typename TType>
	constexpr int kSizeInBits = sizeof(TType) << 3;

	template <>
	struct integer_info<kSizeInBits<std::uint8_t>>
	{
		using UInt = std::uint8_t;
	};

	template <>
	struct integer_info<kSizeInBits<std::uint16_t>>
	{
		using UInt = std::uint16_t;
	};

	template <>
	struct integer_info<kSizeInBits<std::uint32_t>>
	{
		using UInt = std::uint32_t;
	};

	template <>
	struct integer_info<kSizeInBits<std::uint64_t>>
	{
		using UInt = std::uint64_t;
	};

}
