
#pragma once

#include <utility>

namespace mewt::types
{

	template <auto NTag>
	class Opaque
	{
	public:
		using TagType = decltype(NTag);
		using ValueType = decltype(getOpaqueValueType(NTag));
		template <typename... TArgs>
		constexpr explicit Opaque(TArgs&&... args) : _value(std::forward<TArgs>(args)...) {}
		constexpr explicit operator const ValueType&() const { return _value; }
		[[nodiscard]] constexpr auto get() const -> const ValueType& { return _value; }
		constexpr friend auto operator<=>(Opaque lhs, Opaque rhs)
		{
			return lhs._value <=> rhs._value;
		}
		constexpr friend auto operator==(Opaque lhs, Opaque rhs) -> bool
		{
			return lhs._value == rhs._value;
		}

	private:
		ValueType _value;
	};

	template <auto NTag>
	constexpr auto toIndex(Opaque<NTag> lhs)
	{
		constexpr auto index = getOpaqueIndex(NTag);
		return Opaque<index>(lhs.get());
	}

	template <auto NTag>
	constexpr auto operator-(Opaque<NTag> lhs, Opaque<NTag> rhs)
	{
		constexpr auto diff = getOpaqueDifference(NTag);
		return Opaque<diff>(lhs.get() - rhs.get());
	}

	template <auto NTag>
		requires(canIncrement(NTag))
	constexpr auto operator++(Opaque<NTag>& lhs)
	{
		return (lhs = Opaque<NTag>(lhs.get() + 1));
	}

	template <auto NTagL, auto NTagR>
		requires(getOpaqueDifference(NTagL) == NTagR)
	constexpr auto operator-=(Opaque<NTagL>& lhs, Opaque<NTagR> rhs) -> auto&
	{
		lhs = Opaque<NTagL>(lhs.get() - rhs.get());
		return lhs;
	}

	template <auto NTagL, auto NTagR>
		requires(getOpaqueDifference(NTagL) == NTagR)
	constexpr auto operator+=(Opaque<NTagL>& lhs, Opaque<NTagR> rhs) -> auto&
	{
		lhs = Opaque<NTagL>(lhs.get() + rhs.get());
		return lhs;
	}

	template <auto NTagL, auto NTagR>
		requires(getOpaqueDifference(NTagL) == NTagR)
	constexpr auto operator-(Opaque<NTagL> lhs, Opaque<NTagR> rhs)
	{
		return Opaque<NTagL>(lhs.get() - rhs.get());
	}

	template <auto NTagL, auto NTagR>
		requires(getOpaqueDifference(NTagL) == NTagR)
	constexpr auto operator+(Opaque<NTagL> lhs, Opaque<NTagR> rhs)
	{
		return Opaque<NTagL>(lhs.get() + rhs.get());
	}
}
