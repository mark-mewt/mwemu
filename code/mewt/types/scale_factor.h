
#pragma once

#include "mewt/types/opaque.h"

namespace mewt::types {

	class scale_factor_t {
		enum class tags_t {
			Numerator,
			Denominator
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using numerator_t = opaque<tags_t::Numerator>;		//, int>;
		using denominator_t = opaque<tags_t::Denominator>; //, int>;
		static constexpr scale_factor_t Half() { return { numerator_t(1), denominator_t(2) }; };
		template <auto _Tag>
		requires(get_opaque_difference(_Tag) == _Tag) constexpr auto scale(opaque<_Tag> v) const { return opaque<_Tag>((v.get() * _numerator.get()) / _denominator.get()); }

	private:
		constexpr scale_factor_t(numerator_t numerator, denominator_t denominator)
			 : _numerator(numerator), _denominator(denominator) {
		}
		numerator_t _numerator;
		denominator_t _denominator;
	};

}
