
#pragma once

#include "mewt/types/numeric/numeric.space.h"
#include "mewt/types/numeric/value/value.Relative.h"

namespace mewt::types::numeric
{

	template <typename TItem, space::isDiscrete TSpace>
	using Count = value::Relative<TItem, TSpace>;

	template <typename TType, typename TUnit>
	concept isCountOf = value::isRelativeValue<TType> && std::same_as<TUnit, typename TType::Unit>;

}
