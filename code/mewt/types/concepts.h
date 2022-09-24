
#pragma once

#include <concepts>

namespace mewt::types
{

	template<typename _Type>
	concept isEnum = std::is_enum_v<_Type>;

}
