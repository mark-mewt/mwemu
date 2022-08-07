
#pragma once

namespace mewt::types {

	template <auto _Tag>
	class opaque {
	public:
		using tag_type_t = decltype(_Tag);
		using value_type_t = decltype(get_opaque_value_type(_Tag));
		template <typename... _Args>
		constexpr explicit opaque(_Args&&... args) : _value(std::forward<_Args>(args)...) {}
		constexpr explicit operator const value_type_t&() const { return _value; }
		constexpr const value_type_t& get() const { return _value; }
		constexpr friend auto operator<=>(opaque lhs, opaque rhs) {
			return lhs._value <=> rhs._value;
		}

	private:
		value_type_t _value;
	};

	template <auto _Tag>
	constexpr auto operator-(opaque<_Tag> lhs, opaque<_Tag> rhs) {
		constexpr auto diff = get_opaque_difference(_Tag);
		return opaque<diff>(lhs.get() - rhs.get());
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto& operator-=(opaque<_TagL>& lhs, opaque<_TagR> rhs) {
		lhs = opaque<_TagL>(lhs.get() - rhs.get());
		return lhs;
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto& operator+=(opaque<_TagL>& lhs, opaque<_TagR> rhs) {
		lhs = opaque<_TagL>(lhs.get() + rhs.get());
		return lhs;
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto operator-(opaque<_TagL> lhs, opaque<_TagR> rhs) {
		return opaque<_TagL>(lhs.get() - rhs.get());
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto operator+(opaque<_TagL> lhs, opaque<_TagR> rhs) {
		return opaque<_TagL>(lhs.get() + rhs.get());
	}

}
