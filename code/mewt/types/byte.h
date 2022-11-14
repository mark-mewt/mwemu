
#pragma once

#include <cstdint>
#include <type_traits>

namespace mewt::types
{

	constexpr int kRawBitsPerByte = 8;
	constexpr int kOneByteBitMask = 0xff;
	constexpr int kOneNibbleBitMask = 0xf;
	constexpr int kOneWordBitMask = 0xffff;

	constexpr auto highByte(uint16_t word) -> std::uint8_t
	{
		return word >> kRawBitsPerByte;
	}

	constexpr auto lowByte(uint16_t word) -> std::uint8_t
	{
		return word & kOneByteBitMask;
	}

	// constexpr auto lowByte(uint32_t value) -> std::uint8_t { return value & kOneByteBitMask; }
	constexpr auto lowNibble(uint8_t byte) -> std::uint8_t
	{
		return byte & kOneNibbleBitMask;
	}

	constexpr auto highWord(uint32_t value) -> std::uint16_t
	{
		return value >> (kRawBitsPerByte + kRawBitsPerByte);
	}

	constexpr auto lowWord(uint32_t value) -> std::uint16_t
	{
		return value & kOneWordBitMask;
	}

	constexpr auto secondByte(uint32_t value) -> std::uint8_t
	{
		return highByte(lowWord(value));
	}

	constexpr auto thirdByte(uint32_t value) -> std::uint8_t
	{
		return lowByte(highWord(value));
	}

}
