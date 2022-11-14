
#pragma once

#include "mewt/types/numeric/numeric.unit.h"
#include "mewt/types/numeric/value/value.Strong.h"

namespace mewt::types::numeric::value
{

	template <typename TUnit, typename TSpace>
	struct Absolute : public Strong<unit::NativeTypeForUnit<TUnit, TSpace>, Absolute<TUnit, TSpace>>
	{
		using Unit = TUnit;
		using Space = TSpace;

		template <unit::isUnitEquivalent<TUnit, TSpace> TOtherUnit>
		// NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
		constexpr operator Absolute<TOtherUnit, TSpace>() const
		{
			return { innerValue(*this) };
		}

		template <space::isSpaceEquivalent<TSpace> TOtherSpace>
		// NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
		constexpr operator Absolute<TUnit, TOtherSpace>() const
		{
			return { innerValue(*this) };
		}
	};

	template <typename TUnit, typename TSpace>
	constexpr auto operator<=>(Absolute<TUnit, TSpace> lhs, Absolute<TUnit, TSpace> rhs)
	{
		return innerValue(lhs) <=> innerValue(rhs);
	}

	template <typename TTargetSpace, typename TUnit, typename TSourceSpace>
	constexpr auto toSpace(Absolute<TUnit, TSourceSpace> value)
	{
		return Absolute<TUnit, TTargetSpace> { innerValue(value) };
	}

}