
#pragma once

#include <span>

namespace mewt::types {

	template<typename _Type, typename _Width, typename _Height>
	class span_2d_t {

		struct row_range_t;

	public:
		inline span_2d_t(_Type* data, _Width width, _Height height, _Width pitch)
			 : _data(data), _width(width), _height(height), _pitch(pitch) {
		}

		inline auto rows() const {
			return row_range_t{ *this };
		}

	private:

		_Type* _data;
		_Width _width;
		_Height _height;
		_Width _pitch;

		using row_index_t = decltype(to_index(std::declval<_Height>()));

		struct row_proxy_t {
			const span_2d_t& _span;
			row_index_t _row;
			std::span<_Type> _row_span;
			inline bool operator!=(const row_proxy_t& rhs) const { return _row != rhs._row; }
			inline row_proxy_t& operator++() {
				++_row;
				return *this;
			}
			inline auto& operator*() {
				auto* data_start = _span._data + (static_cast<size_t>(_row.get()) * static_cast<size_t>(_span._pitch.get()));
				_row_span = std::span(data_start, data_start + _span._width.get());
				return _row_span;
			}
		};

		struct row_range_t {
			const span_2d_t& _span;
			inline auto begin() const {
				return row_proxy_t{ _span, row_index_t(0) };
			}
			inline auto end() const {
				return row_proxy_t{ _span, to_index(_span._height) };
			}
		};

	};

}
