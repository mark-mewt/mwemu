
#pragma once

#include "mewt/types/opaque.h"

namespace mewt::gfx {

	class Image
	{
		enum class Tags
		{
			X,
			Y,
			Width,
			Height
		};
		constexpr friend auto getOpaqueValueType(Tags) -> int;

	public:
		using XPosition = types::Opaque<Tags::X>;
		using YPosition = types::Opaque<Tags::Y>;
		using Width = types::Opaque<Tags::Width>;
		using Height = types::Opaque<Tags::Height>;
		constexpr friend auto getOpaqueDifference(Tags tag)
		{
			switch (tag) {
			case Tags::X:
				return Tags::Width;
			case Tags::Y:
				return Tags::Height;
			case Tags::Width:
				return Tags::Width;
			case Tags::Height:
				return Tags::Height;
			}
		}
		constexpr friend auto getOpaqueIndex(Tags tag)
		{
			switch (tag) {
			case Tags::X:
				return Tags::X;
			case Tags::Y:
				return Tags::Y;
			case Tags::Width:
				return Tags::X;
			case Tags::Height:
				return Tags::Y;
			}
		}
		constexpr friend auto canIncrement(Tags tag) -> bool
		{
			return (tag == Tags::X) || (tag == Tags::Y);
		}
		struct Position
		{
			XPosition _x;
			YPosition _y;
		};
		struct Size
		{
			Width _width;
			Height _height;
			constexpr static void withComponents(auto fn)
			{
				fn(&Size::_width);
				fn(&Size::_height);
			}
		};
		struct rect_t;
	};
	struct Image::rect_t
	{
		Position _position;
		Size _size;
		[[nodiscard]] constexpr auto left() const -> auto& { return _position._x; }
		[[nodiscard]] constexpr auto right() const { return _position._x + _size._width; } // mwToDo: add proxy to non-const version so we can assign to it
		[[nodiscard]] constexpr auto top() const -> auto& { return _position._y; }
		[[nodiscard]] constexpr auto bottom() const { return _position._y + _size._height; }
	};
	/* constexpr auto operator-(image_t::size_t lhs, image_t::size_t rhs) {
		auto x = lhs._width - rhs._width;
		return image_t::size_t{
			._width = lhs._width - rhs._width,// lhs._width - rhs._width,
			._height = lhs._height - rhs._height
		};
	}*/

}
