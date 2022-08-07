
#pragma once

#include "mewt/types/traits.h"

namespace mewt::types {

	template<typename _Type>
	requires(std::is_same_v<_Type, std::decay_t<_Type>>)
	struct arithmetic_traits {
	};

	template <typename _Type>
	concept IS_custom_arithmetic = std::is_base_of_v<types::arithmetic_traits<std::decay_t<_Type>>, types::traits<_Type>>;

	template <typename _Type>
	concept IS_arithmetic = IS_custom_arithmetic<_Type> || std::is_arithmetic_v<_Type>;

	struct arithmetic_operations_t {

		constexpr static auto subtract(IS_custom_arithmetic auto lhs, IS_custom_arithmetic auto rhs) {
			return copy_of(lhs) -= rhs;  // mwToDo: don't copy when lhs is an r-value
		}
	};

}

//constexpr auto operator-(mewt::types::IS_custom_arithmetic auto lhs, mewt::types::IS_custom_arithmetic auto rhs) {
//	return mewt::types::arithmetic_operations_t::subtract(std::forward<>(lhs), std::forward<>(rhs));
//}

//template <mewt::types::IS_custom_arithmetic _Lhs, mewt::types::IS_custom_arithmetic _Rhs>
template <mewt::types::IS_custom_arithmetic _Lhs, mewt::types::IS_custom_arithmetic _Rhs>
constexpr auto operator - (_Lhs&& lhs, _Rhs&& rhs) {
	return mewt::types::arithmetic_operations_t::subtract(std::forward<_Lhs>(lhs), std::forward<_Rhs>(rhs));
}
