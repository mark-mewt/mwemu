
#pragma once

#include "mewt/types/numeric/value/value.Absolute.h"

namespace mewt::types::numeric::value
{

	template <typename TTransformUnit, typename TSourceSpace, typename TTargetSpace>
	struct Transform
	{
		Absolute<TTransformUnit, TTargetSpace> _transform;
	};

	template <typename TTransformUnit, typename TUnit, typename TSource, typename TTarget>
	constexpr auto operator+(Transform<TTransformUnit, TSource, TTarget> basis,
									 Absolute<TUnit, TSource> source) -> Absolute<TUnit, TTarget>
	{
		return { toRelative(basis._transform) + toSpace<TTarget>(source) };
	}

	template <typename TTransformUnit, typename TUnit, typename TSource, typename TTarget>
	constexpr auto operator*(Transform<TTransformUnit, TSource, TTarget> basis,
									 Absolute<TUnit, TSource> source) -> Absolute<TUnit, TTarget>
	{
		return { basis._transform * toSpace<TTarget>(source) };
	}

}