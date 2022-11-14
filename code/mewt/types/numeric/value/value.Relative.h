
#pragma once

#include "mewt/types/numeric/numeric.unit.h"
#include "mewt/types/numeric/value/value.Strong.h"

namespace mewt::types::numeric::value
{

	template <typename TUnit, typename TSpace>
	struct Relative : public Strong<unit::NativeTypeForUnit<TUnit, TSpace>, Relative<TUnit, TSpace>>
	{
		using Unit = TUnit;
		using Space = TSpace;

		template <unit::isUnitEquivalent<TUnit, TSpace> TOtherUnit>
		// NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
		constexpr operator Relative<TOtherUnit, TSpace>() const
		{
			return { innerValue(*this) };
		}

		template <space::isSpaceEquivalent<TSpace> TOtherSpace>
		// NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
		constexpr operator Relative<TUnit, TOtherSpace>() const
		{
			return { innerValue(*this) };
		}
	};

	template <typename TType>
	concept isRelativeValue = std::same_as<TType, decltype(Relative(TType()))>;

	template <typename TNumerator, typename TDenominator, typename TCommon>
	constexpr auto operator/(Relative<TNumerator, TCommon> lhs, Relative<TDenominator, TCommon> rhs)
	{
		return Relative<TNumerator, TDenominator> { innerValue(lhs) / innerValue(rhs) };
	}

	template <typename TNumerator, typename TDenominator, typename TCommon>
	constexpr auto operator/(Relative<TCommon, TDenominator> lhs, Relative<TCommon, TNumerator> rhs)
	{
		using Result = Relative<TNumerator, TDenominator>;
		return Result { static_cast<typename Result::InnerType>(innerValue(lhs) / innerValue(rhs)) };
	}

	template <typename TNumerator, typename TDenominator, typename TCommon>
	constexpr auto operator*(Relative<TCommon, TDenominator> lhs, Relative<TNumerator, TCommon> rhs)
	{
		using Result = Relative<TNumerator, TDenominator>;
		return Result { static_cast<typename Result::InnerType>(innerValue(lhs) * innerValue(rhs)) };
	}

	template <typename TNumerator, typename TDenominator, typename TCommon>
	constexpr auto operator*(Relative<TNumerator, TCommon> lhs, Relative<TCommon, TDenominator> rhs)
	{
		using Result = Relative<TNumerator, TDenominator>;
		return Result { static_cast<typename Result::InnerType>(innerValue(lhs) * innerValue(rhs)) };
	}

	template <typename TUnit, typename TSpace>
	constexpr auto operator+(Relative<TUnit, TSpace> lhs, Relative<TUnit, TSpace> rhs)
	{
		return Relative<TUnit, TSpace> { innerValue(lhs) + innerValue(rhs) };
	}

	template <typename TUnit, typename TLeftSpace, typename TRightSpace>
	constexpr auto operator<=>(Relative<TUnit, TLeftSpace> lhs, Relative<TUnit, TRightSpace> rhs)
	{
		return innerValue(lhs) <=> innerValue(rhs);
	}

	template <typename TTargetSpace, typename TUnit, typename TSourceSpace>
	constexpr auto toSpace(Relative<TUnit, TSourceSpace> value)
	{
		return Relative<TUnit, TTargetSpace> { innerValue(value) };
	}

	template <typename TLeftUnit, typename TRightUnit, typename TSpace,
				 typename TCommon = std::common_reference_t<TLeftUnit, TRightUnit>>
	constexpr auto operator+(Relative<TLeftUnit, TSpace> lhs, Relative<TRightUnit, TSpace> rhs)
	{
		return Relative<TCommon, TSpace> { innerValue(lhs) + innerValue(rhs) };
	}

}