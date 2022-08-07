
#pragma once

#include "mewt/types/int_types.h"
#include "mewt/types/concepts.h"
#include "mewt/types/traits.h"

namespace mewt::types
{

	template <IS_Enum _Enum>
	class flags;

	template <IS_Enum _Enum>
	struct flags_traits : public default_traits<flags<_Enum>> {
		constexpr static int BitCount = sizeof(std::underlying_type_t<_Enum>) * 8;
	};

	template <IS_Enum _Enum>
	constexpr auto get_type_traits(types::class_id<flags<_Enum>>) -> types::class_id<flags_traits<_Enum>>;

	struct no_flags_t {};

	template<typename _Type>
	struct type_tester;

	template <IS_Enum _Enum>
	class flags
	{

		//type_tester<decltype(types::dummy_arg<flags>())> _tester1;
		//type_tester<flags> _tester2;
		//type_tester<decltype(get_type_traits(types::dummy_arg<flags>()))> _tester3;

		using data_t = uint<traits<flags>::BitCount>;

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

		constexpr explicit flags(data_t bits) : _bits(bits) { }
		constexpr flags(no_flags_t) { }
		constexpr flags(_Enum en) { (*this)[en] = true; }

		constexpr explicit operator data_t () const { return _bits; }

		constexpr data_t& raw_bits() { return _bits; }
		constexpr const data_t& raw_bits() const { return _bits; }

		constexpr proxy operator [] (_Enum en) { return proxy{ *this, (data_t)(1 << (data_t)en) }; }
		constexpr bool operator [] (_Enum en) const { return _bits & (1 << (data_t)en); }

		constexpr auto& operator|=(_Enum en) {
			(*this)[en] = true;
			return *this;
		}
		constexpr flags operator|(_Enum en) {
			return flags(*this) |= en;
		}

	private:

		data_t _bits = 0;

	};

	template<IS_Enum _Enum>
	constexpr auto operator|(_Enum lhs, _Enum rhs) { return flags(lhs) | rhs; }

}
