
#pragma once

#include "mewt/types/int_types.h"
#include "mewt/types/numeric/numeric.Count.h"

namespace mewt::types
{

	template <auto NBitCount>
		requires numeric::isCountOf<decltype(NBitCount), native::Bit>
	class BitField
	{
		using Data = numeric::Integer<NBitCount, false>;

		class Proxy
		{
		public:
			constexpr explicit operator bool() const
			{
				return _bitfield._bits & _mask;
			}

			constexpr auto operator=(bool bit) -> Proxy&
			{
				if (bit)
					_bitfield._bits |= _mask;
				else
					_bitfield._bits &= ~_mask;
				return *this;
			}

		private:
			BitField& _bitfield;
			Data _mask;
		};

	public:
		constexpr BitField()
			 : _bits(0)
		{
		}

		constexpr explicit BitField(Data bits)
			 : _bits(bits)
		{
		}

		constexpr explicit operator Data() const
		{
			return _bits;
		}

		constexpr auto operator[](Data bit) -> Proxy
		{
			return Proxy { *this, static_cast<Data>(1 << bit) };
		}

		constexpr auto operator[](Data bit) const -> bool
		{
			return _bits & (1 << bit);
		}

	private:
		Data _bits;
	};

}
