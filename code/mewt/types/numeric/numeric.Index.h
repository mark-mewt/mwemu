
#pragma once

#include "mewt/types/numeric/numeric.space.h"
#include "mewt/types/numeric/value/value.Absolute.h"

namespace mewt::types::numeric
{

	template <typename TItem, space::isDiscrete TSpace>
	using Index = value::Absolute<TItem, TSpace>;

}
