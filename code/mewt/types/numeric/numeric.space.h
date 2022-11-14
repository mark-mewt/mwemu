
#pragma once

#include "mewt/types/traits/traits.TypeId.h"

#include <concepts>

namespace mewt::types::numeric::space
{

	enum class Granularity
	{
		Discrete,
		Continuous
	};

	struct Discrete
	{
	};

	template <typename TSpace>
	concept isSpace = requires {
								{
									getNumericSpaceGranularity(traits::kTypeId<TSpace>)
									} -> std::same_as<Granularity>;
							};

	template<std::derived_from<Discrete> TDiscrete>
	constexpr auto getNumericSpaceGranularity(traits::TypeId<TDiscrete> /*unused*/)
	{
		return Granularity::Discrete;
	}

	template <typename TSpace>
	concept isDiscrete = (getNumericSpaceGranularity(traits::kTypeId<TSpace>) ==
								 Granularity::Discrete);

	template <typename TLeftSpace, typename TRightSpace>
	concept isSpaceEquivalent = declareEquivalentSpaces(traits::kTypeId<TLeftSpace>,
																		 traits::kTypeId<TRightSpace>) ||
										 declareEquivalentSpaces(traits::kTypeId<TRightSpace>,
																		 traits::kTypeId<TLeftSpace>);

}