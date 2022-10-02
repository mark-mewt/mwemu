
#pragma once

#include <span>

namespace mewt::types
{

	template <typename TType, typename TWidth, typename THeight>
	class Span2D
	{

		struct RowRange;

	public:
		inline Span2D(std::span<TType> data, TWidth width, THeight height, TWidth pitch)
			 : _data(data), _width(width), _height(height), _pitch(pitch)
		{
		}

		[[nodiscard]] inline auto rows() const
		{
			return RowRange{ *this };
		}

	private:
		std::span<TType> _data;
		TWidth _width;
		THeight _height;
		TWidth _pitch;

		using RowIndex = decltype(toIndex(std::declval<THeight>()));

		struct RowProxy
		{
			const Span2D& _span;
			RowIndex _row;
			std::span<TType> _row_span;
			inline friend auto operator!=(const RowProxy& lhs, const RowProxy& rhs) -> bool { return lhs._row != rhs._row; }
			inline friend auto operator==(const RowProxy& lhs, const RowProxy& rhs) -> bool { return lhs._row == rhs._row; }
			inline friend auto operator++(RowProxy& proxy) -> RowProxy&
			{
				++proxy._row;
				return proxy;
			}
			inline friend auto operator*(RowProxy& proxy) -> auto&
			{
				proxy._row_span = proxy._span._data.subspan(static_cast<size_t>(proxy._row.get()) * static_cast<size_t>(proxy._span._pitch.get()), proxy._span._width.get());
				// auto* data_start = proxy._span._data + (static_cast<size_t>(proxy._row.get()) * static_cast<size_t>(proxy._span._pitch.get()));
				// proxy._row_span = std::span(data_start, proxy._span._width.get());
				return proxy._row_span;
			}
		};

		struct RowRange
		{
			const Span2D& _span;
			[[nodiscard]] inline friend auto begin(const RowRange& range)
			{
				return RowProxy{ range._span, RowIndex(0) };
			}
			[[nodiscard]] inline friend auto end(const RowRange& range)
			{
				return RowProxy{ range._span, toIndex(range._span._height) };
			}
		};
	};
}
