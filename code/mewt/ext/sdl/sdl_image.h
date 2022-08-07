
#pragma once

#include "mewt/types/opaque.h"

namespace mewt::ext::sdl {

	class image_t {
		enum class tags_t {
			X,
			Y,
			Width,
			Height
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using x_position_t = types::opaque<tags_t::X>;
		using y_position_t = types::opaque<tags_t::Y>;
		using width_t = types::opaque<tags_t::Width>;
		using height_t = types::opaque<tags_t::Height>;
		constexpr friend auto get_opaque_difference(tags_t tag) {
			switch (tag) {
			case tags_t::X:
				return tags_t::Width;
			case tags_t::Y:
				return tags_t::Height;
			case tags_t::Width:
				return tags_t::Width;
			case tags_t::Height:
				return tags_t::Height;
			}
		}
		struct position_t {
			x_position_t _x;
			y_position_t _y;
		};
		struct size_t {
			width_t _width;
			height_t _height;
			constexpr static void with_components(auto fn) {
				fn(&size_t::_width);
				fn(&size_t::_height);
			}
		};
		struct rect_t;
	};
	struct image_t::rect_t {
		position_t _position;
		size_t _size;
		constexpr auto& left() const { return _position._x; }
		constexpr auto right() const { return _position._x + _size._width; } // mwToDo: add proxy to non-const version so we can assign to it
		constexpr auto& top() const { return _position._y; }
		constexpr auto bottom() const { return _position._y + _size._height; }
	};
	/* constexpr auto operator-(image_t::size_t lhs, image_t::size_t rhs) {
		auto x = lhs._width - rhs._width;
		return image_t::size_t{
			._width = lhs._width - rhs._width,// lhs._width - rhs._width,
			._height = lhs._height - rhs._height
		};
	}*/

}
