
#pragma once

namespace mewt::types {

	template <auto TTag>
	class Opaque
	{
	public:
		using TagType = decltype(TTag);
		using ValueType = decltype(getOpaqueValueType(TTag));
		template <typename... _Args>
		constexpr explicit Opaque(_Args&&... args) : _value(std::forward<_Args>(args)...) {}
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

	template <auto TTag>
	constexpr auto toIndex(Opaque<TTag> lhs)
	{
		constexpr auto kIndex = getOpaqueIndex(TTag);
		return Opaque<kIndex>(lhs.get());
	}

	template <auto TTag>
	constexpr auto operator-(Opaque<TTag> lhs, Opaque<TTag> rhs)
	{
		constexpr auto kDiff = getOpaqueDifference(TTag);
		return Opaque<kDiff>(lhs.get() - rhs.get());
	}

	template <auto TTag>
		requires(canIncrement(TTag))
	constexpr auto operator++(Opaque<TTag>& lhs)
	{
		return (lhs = Opaque<TTag>(lhs.get() + 1));
	}

	template <auto TTagL, auto TTagR>
	requires(getOpaqueDifference(TTagL) == TTagR)
	constexpr auto operator-=(Opaque<TTagL>& lhs, Opaque<TTagR> rhs) -> auto&
	{
			lhs = Opaque<TTagL>(lhs.get() - rhs.get());
		return lhs;
	}

	template <auto TTagL, auto TTagR>
	requires(getOpaqueDifference(TTagL) == TTagR) constexpr auto operator+=(Opaque<TTagL>& lhs, Opaque<TTagR> rhs) -> auto&
	{
		lhs = Opaque<TTagL>(lhs.get() + rhs.get());
		return lhs;
	}

	template <auto TTagL, auto TTagR>
	requires(getOpaqueDifference(TTagL) == TTagR) constexpr auto operator-(Opaque<TTagL> lhs, Opaque<TTagR> rhs)
	{
		return Opaque<TTagL>(lhs.get() - rhs.get());
	}

	template <auto TTagL, auto TTagR>
	requires(getOpaqueDifference(TTagL) == TTagR)
	constexpr auto operator+(Opaque<TTagL> lhs, Opaque<TTagR> rhs)
	{
		return Opaque<TTagL>(lhs.get() + rhs.get());
	}
}
