
#pragma once

#include "mewt/types/native/native.Bit.h"
#include "mewt/types/native/native.Integer.decl.h"
#include "mewt/types/numeric/numeric.Count.h"
#include "mewt/types/traits/traits.Layout.h"
#include "mewt/types/native/native.Value.h"

namespace mewt::types::native
{

	constexpr auto isSafeIntegerValueType(traits::TypeId<Byte>) { return true; }

	constexpr auto isSafeIntegerValueType(traits::TypeId<Word>) { return true; }

	constexpr auto isSafeIntegerValueType(traits::TypeId<DWord>) { return true; }

	constexpr auto isSafeIntegerValueType(traits::TypeId<QWord>) { return true; }

	template <typename TType>
	concept isSafeInteger = isSafeIntegerValueType(traits::kTypeId<TType>);

	template <isSafeInteger TValueType>
	constexpr auto getNumericSpaceGranularity(traits::TypeId<TValueType>)
	{
		return numeric::space::Granularity::Discrete;
	}

	template <isSafeInteger TValueType>
	constexpr auto getNativeTypeForUnit(traits::TypeId<Bit>, traits::TypeId<TValueType>)
		 -> traits::TypeId<std::uint8_t>;
	template <isSafeInteger TValueType>
	constexpr auto getNativeTypeForUnit(traits::TypeId<Byte>, traits::TypeId<TValueType>)
		 -> traits::TypeId<std::uint8_t>;

	template <isSafeInteger TValueType>
	constexpr auto declareTypeAsLayoutSpace(traits::TypeId<TValueType>)
	{
		return true;
	}

	constexpr numeric::Count<Bit, Byte> kBitsPerByte { 8 };
	constexpr auto kBitsPerWord = kBitsPerByte * traits::kSizeOf<Word>;
	constexpr auto kBitsPerDWord = kBitsPerByte * traits::kSizeOf<DWord>;
	constexpr auto kBitsPerQWord = kBitsPerByte * traits::kSizeOf<QWord>;

	template <isSafeInteger TValueType>
	constexpr auto nativeValue(TValueType value)
	{
		using ReturnType = std::underlying_type_t<TValueType>;
		return static_cast<ReturnType>(value);
	}

	template <isSafeInteger TValueType>
	constexpr auto operator~(TValueType value) -> TValueType
	{
		auto nv = nativeValue(value);
		nv = ~nv;
		return TValueType { nv };
	}

	template <isSafeInteger TValueType>
	constexpr auto operator+(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) + nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator+=(TValueType& lhs, TValueType rhs) -> TValueType&
	{
		return (lhs = lhs + rhs);
	}

	template <isSafeInteger TValueType>
	constexpr auto operator-(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) - nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator-=(TValueType& lhs, TValueType rhs) -> TValueType&
	{
		return (lhs = lhs - rhs);
	}

	template <isSafeInteger TValueType>
	constexpr auto operator--(TValueType& value) -> TValueType&
	{
		auto inner = native::valueOf(value);
		--inner;
		value = TValueType { inner };
		return value;
	}

	template <isSafeInteger TValueType>
	constexpr auto operator++(TValueType& value) -> TValueType&
	{
		auto inner = native::valueOf(value);
		++inner;
		value = TValueType { inner };
		return value;
	}

	template <isSafeInteger TValueType>
	constexpr auto operator*(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) * nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator/(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) / nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator^(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) ^ nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator^=(TValueType& lhs, TValueType rhs) -> TValueType&
	{
		lhs = lhs ^ rhs;
		return lhs;
	}

	// native integers should not be mixed with numeric counts at this level.
	// native types => safe enum types => strong unit types => metric types etc
	template <isSafeInteger TValueType>
	constexpr auto operator<<(TValueType lhs, numeric::isCountOf<Bit> auto rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) << innerValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator>>(TValueType lhs, numeric::isCountOf<Bit> auto rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) >> innerValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator|(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) | nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator|=(TValueType& lhs, TValueType rhs) -> TValueType&
	{
		return (lhs = lhs | rhs);
	}

	template <isSafeInteger TValueType>
	constexpr auto operator&(TValueType lhs, TValueType rhs)
	{
		return static_cast<TValueType>(nativeValue(lhs) & nativeValue(rhs));
	}

	template <isSafeInteger TValueType>
	constexpr auto operator&=(TValueType& lhs, TValueType rhs) -> TValueType&
	{
		return (lhs = lhs & rhs);
	}

	constexpr auto toWord(Byte value) { return static_cast<Word>(value); }

	constexpr auto makeWord(Byte low, Byte high) -> Word
	{
		return (toWord(high) << kBitsPerByte) | toWord(low);
	}

	constexpr auto bitsRequiredForValue(numeric::value::isStrongValue auto value)
	{
		return numeric::Count<native::Bit, traits::Layout<decltype(value)>> { static_cast<std::uint8_t>
			
			(std::bit_width(
			 native::valueOf(value))) };
	}

}