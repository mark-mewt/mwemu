
#pragma once

#include <concepts>

namespace mewt::types
{

	template<typename _Type>
	concept IS_Enum = std::is_enum_v<_Type>;

}
