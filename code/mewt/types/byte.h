
#pragma once

#include <cstdint>

namespace mewt::types
{

	constexpr int kBitsPerByte = 8;
	constexpr int kOneByteBitMask = 0xff;
	constexpr int kOneNibbleBitMask = 0xf;
	constexpr int kOneWordBitMask = 0xffff;
	constexpr auto highByte(uint16_t word) -> std::uint8_t { return word >> kBitsPerByte; }
	constexpr auto lowByte(uint16_t word) -> std::uint8_t { return word & kOneByteBitMask; }
	// constexpr auto lowByte(uint32_t value) -> std::uint8_t { return value & kOneByteBitMask; }
	constexpr auto lowNibble(uint8_t byte) -> std::uint8_t { return byte & kOneNibbleBitMask; }
	constexpr auto makeWord(uint8_t low, uint8_t high) -> std::uint16_t { return (static_cast<uint16_t>(high) << kBitsPerByte) | low; }
	constexpr auto highWord(uint32_t value) -> std::uint16_t { return value >> (kBitsPerByte + kBitsPerByte); }
	constexpr auto lowWord(uint32_t value) -> std::uint16_t { return value & kOneWordBitMask; }
	constexpr auto secondByte(uint32_t value) -> std::uint8_t { return highByte(lowWord(value)); }
	constexpr auto thirdByte(uint32_t value) -> std::uint8_t { return lowByte(highWord(value)); }

}
