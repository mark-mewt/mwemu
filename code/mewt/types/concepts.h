
#pragma once

#include <concepts>

namespace mewt::types
{

	template <typename TType>
	concept isEnum = std::is_enum_v<TType>;

}
