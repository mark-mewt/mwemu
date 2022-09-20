
#pragma once

#include <span>

namespace mewt::types {

	template <typename _Type, typename _Width, typename _Height>
	class Span2dT
	{

		struct row_range_t;

	public:
		inline Span2dT(_Type* data, _Width width, _Height height, _Width pitch)
			 : _data(data), _width(width), _height(height), _pitch(pitch)
		{
		}

		[[nodiscard]] inline auto rows() const
		{
			return RowRangeT{ *this };
		}

	private:

		_Type* _data;
		_Width _width;
		_Height _height;
		_Width _pitch;

		using RowIndex = decltype(toIndex(std::declval<_Height>()));

		struct row_proxy_t {
			const Span2dT& _span;
			RowIndex _row;
			std::span<_Type> _row_span;
			inline auto operator!=(const row_proxy_t& rhs) const -> bool { return _row != rhs._row; }
			inline auto operator==(const row_proxy_t& rhs) const -> bool { return _row == rhs._row; }
			inline auto operator++() -> row_proxy_t&
			{
				++_row;
				return *this;
			}
			inline auto operator*() -> auto&
			{
				auto* data_start = _span._data + (static_cast<size_t>(_row.get()) * static_cast<size_t>(_span._pitch.get()));
				_row_span = std::span(data_start, data_start + _span._width.get());
				return _row_span;
			}
		};

		struct RowRangeT
		{
			const Span2dT& _span;
			[[nodiscard]] inline auto begin() const
			{
				return row_proxy_t{ _span, RowIndex(0) };
			}
			[[nodiscard]] inline auto end() const
			{
				return row_proxy_t{ _span, toIndex(_span._height) };
			}
		};
	};
}
