
#pragma once

#include "mewt/types/int_types.h"

namespace mewt::types {

	template <int _Bits>
	class bitfield {

		using data_t = uint<_Bits>;

		struct proxy {
			bitfield& _bitfield;
			data_t _mask;
			constexpr operator bool() const { return _bitfield._bits & _mask; }
			constexpr proxy& operator=(bool bit) {
				if (bit)
					_bitfield._bits |= _mask;
				else
					_bitfield._bits &= ~_mask;
				return *this;
			}
		};

	public:
		constexpr bitfield(data_t bits = 0) : _bits(bits) {}

		constexpr operator data_t() const { return _bits; }

		constexpr proxy operator[](data_t v) { return proxy{ *this, (data_t)(1 << v) }; }
		constexpr bool operator[](data_t v) const { return _bits & (1 << v); }

	private:
		data_t _bits;
	};

}
