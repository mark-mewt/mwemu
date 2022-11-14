
#pragma once

#include "mewt/types/numeric/value/value.Absolute.h"
#include "mewt/types/numeric/value/value.Relative.h"

namespace mewt::types::numeric::value
{

	template <typename TUnit, typename TSpace>
	constexpr auto operator+(Absolute<TUnit, TSpace> lhs, Relative<TUnit, TSpace> rhs)
	{
		return Absolute<TUnit, TSpace> { innerValue(lhs) + innerValue(rhs) };
	}

	template <typename TUnit, typename TSpace>
	constexpr auto operator+(Relative<TUnit, TSpace> lhs, Absolute<TUnit, TSpace> rhs)
	{
		return Absolute<TUnit, TSpace> { innerValue(lhs) + innerValue(rhs) };
	}

	template <typename TUnit, typename TSpace>
	constexpr auto toRelative(Absolute<TUnit, TSpace> value)
	{
		return Relative<TUnit, TSpace> { innerValue(value) };
	}

}