
#pragma once

#include "mewt/types/flags.decl.h"
#include "mewt/types/concepts.h"
#include "mewt/types/int_types.h"
#include "mewt/types/traits.h"

namespace mewt::types
{

	template <typename _Enum>
	struct FlagsTraits : public DefaultTraits<flags<_Enum>>
	{
		constexpr static int BitCount = sizeof(std::underlying_type_t<_Enum>) * 8;
	};

	template <typename _Enum>
	constexpr auto getTypeTraits(types::ClassId<flags<_Enum>>) -> types::ClassId<FlagsTraits<_Enum>>;

	struct NoFlags
	{
	};

	template<typename _Type>
	struct type_tester;

	template <typename _Enum>
	class flags
	{

		static_assert(isEnum<_Enum>, "Only enum types allowed in flags.");

		//type_tester<decltype(types::dummy_arg<flags>())> _tester1;
		//type_tester<flags> _tester2;
		//type_tester<decltype(get_type_traits(types::dummy_arg<flags>()))> _tester3;

		using Data = uint<Traits<flags>::BitCount>;

		struct Proxy
		{
			flags& _flags;
			Data _mask;
			constexpr operator bool() const { return _flags._bits & _mask; }
			constexpr auto operator=(bool bit) -> Proxy&
			{
				if (bit)
					_flags._bits |= _mask;
				else
					_flags._bits &= ~_mask;
				return *this;
			}
		};

	public:
		constexpr explicit flags(Data bits) : _bits(bits) {}
		constexpr flags(NoFlags /*unused*/) {}
		constexpr flags(_Enum en) { (*this)[en] = true; }

		constexpr explicit operator Data () const { return _bits; }

		constexpr auto rawBits() -> Data& { return _bits; }
		[[nodiscard]] constexpr auto rawBits() const -> const Data& { return _bits; }

		constexpr Proxy operator [] (_Enum en) { return Proxy{ *this, (Data)(1 << (Data)en) }; }
		constexpr auto operator[](_Enum en) const -> bool { return _bits & (1 << (Data)en); }

		constexpr auto operator|=(_Enum en) -> auto&
		{
			(*this)[en] = true;
			return *this;
		}
		constexpr auto operator|(_Enum en) -> flags
		{
			return flags(*this) |= en;
		}

		[[nodiscard]] constexpr auto intersect(flags rhs) const -> flags { return flags(_bits & rhs._bits); }

		[[nodiscard]] constexpr auto isEmpty() const -> bool { return _bits == 0; }

	private:
		Data _bits = 0;
	};

	template <typename _Enum>
	constexpr auto operator|(_Enum lhs, _Enum rhs) { return flags(lhs) | rhs; }

}
