
#pragma once

namespace mewt::types {

	template <auto _Value, typename _Enum = decltype(_Value)>
	struct enum_value {};

}
