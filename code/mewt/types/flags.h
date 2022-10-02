
#pragma once

#include "mewt/types/concepts.h"
#include "mewt/types/flags.decl.h"
#include "mewt/types/int_types.h"
#include "mewt/types/traits.h"

namespace mewt::types
{

	template <typename TEnum>
	struct FlagsTraits : public DefaultTraits<flags<TEnum>>
	{
		constexpr static int kBitCount = sizeof(std::underlying_type_t<TEnum>) * 8;
	};

	template <typename TEnum>
	constexpr auto getTypeTraits(types::ClassId<flags<TEnum>>) -> types::ClassId<FlagsTraits<TEnum>>;

	struct NoFlags
	{
	};

	template <typename TType>
	struct type_tester;

	template <typename TEnum>
	class flags
	{

		static_assert(isEnum<TEnum>, "Only enum types allowed in flags.");

		// type_tester<decltype(types::dummy_arg<flags>())> _tester1;
		// type_tester<flags> _tester2;
		// type_tester<decltype(get_type_traits(types::dummy_arg<flags>()))> _tester3;

		using Data = UInt<Traits<flags>::kBitCount>;

		class Proxy
		{
		public:
			constexpr operator int() const = delete;
			constexpr operator bool() const { return _flags._bits & _mask; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
			constexpr auto operator=(bool bit) -> Proxy&
			{
				if (bit)
					_flags._bits |= _mask;
				else
					_flags._bits &= ~_mask;
				return *this;
			}

		private:
			friend flags;
			Proxy(flags& flags, Data mask) : _flags(flags), _mask(mask) {}
			flags& _flags;
			Data _mask;
		};

	public:
		constexpr explicit flags(Data bits) : _bits(bits) {}
		constexpr flags(NoFlags /*unused*/) {}						// NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
		constexpr flags(TEnum flag) { (*this)[flag] = true; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

		constexpr explicit operator Data() const { return _bits; }

		constexpr auto rawBits() -> Data& { return _bits; }
		[[nodiscard]] constexpr auto rawBits() const -> const Data& { return _bits; }

		constexpr auto operator[](TEnum flag) -> Proxy { return Proxy{ *this, static_cast<Data>(1 << static_cast<Data>(flag)) }; }
		constexpr auto operator[](TEnum flag) const -> bool { return _bits & (1 << static_cast<Data>(flag)); }

		constexpr auto operator|=(TEnum flag) -> auto&
		{
			(*this)[flag] = true;
			return *this;
		}
		constexpr auto operator|(TEnum flag) -> flags
		{
			return flags(*this) |= flag;
		}

		[[nodiscard]] constexpr auto intersect(flags rhs) const -> flags { return flags(_bits & rhs._bits); }

		[[nodiscard]] constexpr auto isEmpty() const -> bool { return _bits == 0; }

	private:
		Data _bits = 0;
	};

	template <typename TEnum>
	constexpr auto operator|(TEnum lhs, TEnum rhs) { return flags(lhs) | rhs; }

}
