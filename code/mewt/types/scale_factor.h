
#pragma once

#include "mewt/types/opaque.h"

namespace mewt::types {

	class scale_factor_t {
		enum class tags_t {
			Numerator,
			Denominator
		};
		constexpr friend auto getOpaqueValueType(tags_t) -> int;

	public:
		using numerator_t = Opaque<tags_t::Numerator>;		//, int>;
		using denominator_t = Opaque<tags_t::Denominator>; //, int>;
		static constexpr scale_factor_t Half() { return { numerator_t(1), denominator_t(2) }; };
		template <auto _Tag>
		requires(getOpaqueDifference(_Tag) == _Tag) constexpr auto scale(Opaque<_Tag> v) const { return Opaque<_Tag>((v.get() * _numerator.get()) / _denominator.get()); }

	private:
		constexpr scale_factor_t(numerator_t numerator, denominator_t denominator)
			 : _numerator(numerator), _denominator(denominator) {
		}
		numerator_t _numerator;
		denominator_t _denominator;
	};

}
