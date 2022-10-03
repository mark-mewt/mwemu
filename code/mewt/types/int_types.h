
#pragma once

#include <cstdint>
#include <span>

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

	template <int NBitWidth>
	struct OpaqueInfo
	{
		static constexpr auto underlyingType()
		{
			if constexpr (NBitWidth <= kSizeInBits<std::uint8_t>)
				return std::uint8_t{ 0 };
			else if constexpr (NBitWidth <= kSizeInBits<std::uint16_t>)
				return std::uint16_t{ 0 };
			else if constexpr (NBitWidth <= kSizeInBits<std::uint32_t>)
				return std::uint32_t{ 0 };
			else if constexpr (NBitWidth <= kSizeInBits<std::uint64_t>)
				return std::uint64_t{ 0 };
		}
		using UnderlyingType = decltype(underlyingType());
		enum class Enum : UnderlyingType
		{
		};
		constexpr friend auto isOpaqueInt(Enum) -> std::true_type;
	};

	template <int NBitWidth>
	using OpaqueInt = typename OpaqueInfo<NBitWidth>::Enum;

	template <typename TType>
	concept is_OpaqueInt = requires(TType value) {
									  {
										  isOpaqueInt(value)
										  } -> std::same_as<std::true_type>;
								  };

	using HalfByte = OpaqueInt<4>;
	using Byte = OpaqueInt<kSizeInBits<std::uint8_t>>;
	using Word = OpaqueInt<kSizeInBits<std::uint16_t>>;
	using DWord = OpaqueInt<kSizeInBits<std::uint32_t>>;
	using QWord = OpaqueInt<kSizeInBits<std::uint64_t>>;

	template <typename TType>
	constexpr auto asUnderlyingType(TType value)
		 -> std::underlying_type_t<TType>
	{
		return static_cast<std::underlying_type_t<TType>>(value);
	}
	template <typename TType>
	constexpr auto fromUnderlyingType(std::underlying_type_t<TType> value)
		 -> TType
	{
		return static_cast<TType>(value);
	}

	template <is_OpaqueInt TType>
	constexpr auto operator++(TType& value) -> TType&
	{
		value = fromUnderlyingType<TType>(asUnderlyingType(value) + 1);
		return value;
	}

	template <is_OpaqueInt TType>
	constexpr auto operator--(TType& value) -> TType&
	{
		value = fromUnderlyingType<TType>(asUnderlyingType(value) - 1);
		return value;
	}

	constexpr auto operator+(Word word, Byte byte) -> Word { return fromUnderlyingType<Word>(asUnderlyingType(word) + asUnderlyingType(byte)); }
	constexpr auto operator+(Byte lhs, Byte rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) + asUnderlyingType(rhs)); }

	constexpr auto operator-(Byte lhs, Byte rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) - asUnderlyingType(rhs)); }

	constexpr auto operator-=(Byte& lhs, Byte rhs) { lhs = lhs - rhs; }
	constexpr auto operator+=(Byte& lhs, Byte rhs) { lhs = lhs + rhs; }

	// constexpr auto operator|=(Byte& lhs, Byte rhs) { lhs = fromUnderlyingType<Byte>(asUnderlyingType(lhs) | asUnderlyingType(rhs)); }
	// constexpr auto operator&=(Byte& lhs, Byte rhs) { lhs = fromUnderlyingType<Byte>(asUnderlyingType(lhs) & asUnderlyingType(rhs)); }

	template <is_OpaqueInt TType>
	constexpr auto operator&(TType lhs, TType rhs) -> TType { return fromUnderlyingType<TType>(asUnderlyingType(lhs) & asUnderlyingType(rhs)); }
	constexpr auto operator|(Byte lhs, Byte rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) | asUnderlyingType(rhs)); }
	constexpr auto operator^(Byte lhs, Byte rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) ^ asUnderlyingType(rhs)); }

	template <is_OpaqueInt TType>
	constexpr auto operator&=(TType& lhs, TType rhs) -> TType&
	{
		lhs = lhs & rhs;
		return lhs;
	}

	template <is_OpaqueInt TType>
	constexpr auto operator|=(TType& lhs, TType rhs) -> TType&
	{
		lhs = lhs | rhs;
		return lhs;
	}

	constexpr auto operator^=(Byte& lhs, Byte rhs) { lhs = lhs ^ rhs; }

	constexpr auto operator~(Byte byte) -> Byte { return fromUnderlyingType<Byte>(~asUnderlyingType(byte)); }

	constexpr auto operator<<(Byte lhs, int rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) << rhs); }
	constexpr auto operator>>(Byte lhs, int rhs) -> Byte { return fromUnderlyingType<Byte>(asUnderlyingType(lhs) >> rhs); }

	constexpr auto makeWord(Byte low, Byte high) -> Word { return static_cast<Word>((static_cast<uint16_t>(high) << kSizeInBits<std::uint8_t>) | static_cast<uint16_t>(low)); }

	template <int NBits, typename TType>
	constexpr auto lowBits(TType value)
		 -> OpaqueInt<NBits>
	{
		return fromUnderlyingType<OpaqueInt<NBits>>(asUnderlyingType(value) & ((1 << NBits) - 1));
	}

	template <int NBits, typename TType>
	constexpr auto highBits(TType value)
		 -> OpaqueInt<NBits>
	{
		return fromUnderlyingType<OpaqueInt<NBits>>(asUnderlyingType(value) >> (kSizeInBits<TType> - NBits));
	}

	template <typename TType>
	constexpr auto lowNibble(TType value) { return lowBits<4>(value); }

	constexpr auto lowByte(OpaqueInt<16> value)
		 -> OpaqueInt<8>
	{
		return fromUnderlyingType<OpaqueInt<8>>(asUnderlyingType(value) & 0xff);
	}
	constexpr auto highByte(OpaqueInt<16> value)
		 -> OpaqueInt<8>
	{
		return fromUnderlyingType<OpaqueInt<8>>(asUnderlyingType(value) >> 8);
	}
	constexpr auto toWord(OpaqueInt<8> value) -> OpaqueInt<16>
	{
		return fromUnderlyingType<OpaqueInt<16>>(asUnderlyingType(value));
	}

	class Zero
	{
	public:
		template <typename TType>
		constexpr operator TType() const { return fromUnderlyingType<TType>(0); }
	};

	constexpr Zero zero;

	// #mwToDo: Move elsewhere
	template <std::size_t NExtent>
	class ByteSpan : public std::span<Byte, NExtent>
	{
		using Super = std::span<Byte, NExtent>;

	public:
		template <typename TType>
		inline ByteSpan(TType& object)
			 : Super(static_cast<Byte*>(static_cast<void*>(std::addressof(object))), sizeof(TType))
		{
			static_assert(sizeof(TType) == NExtent, "ByteSpan must cover the object exactly.");
		}
		using Address = types::OpaqueInt<std::bit_width(NExtent - 1)>;
		inline const auto operator[](Address address) const
		{
			return Super::operator[](asUnderlyingType(address));
		}
		inline auto& operator[](Address address)
		{
			return Super::operator[](asUnderlyingType(address));
		}
	};

	template <typename TType>
	ByteSpan(TType) -> ByteSpan<sizeof(TType)>;

}
