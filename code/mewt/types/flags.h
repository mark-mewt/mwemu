
#pragma once

#include "mewt/types/int_types.h"

namespace mewt::types
{

	template<typename _Enum, int _Bits>
	class flags
	{

		using data_t = uint<_Bits>;

		struct proxy
		{
			flags& _flags;
			data_t _mask;
			constexpr operator bool() const { return _flags._bits & _mask; }
			constexpr proxy& operator = (bool bit)
			{
				if (bit)
					_flags._bits |= _mask;
				else
					_flags._bits &= ~_mask;
				return *this;
			}
		};

	public:

		constexpr flags(data_t bits) : _bits(bits) { }

		constexpr operator data_t () const { return _bits; }

		constexpr proxy operator [] (_Enum en) { return proxy{ *this, (data_t)(1 << (data_t)en) }; }
		constexpr bool operator [] (_Enum en) const { return _bits & (1 << (data_t)en); }

	private:

		data_t _bits;

	};

}
