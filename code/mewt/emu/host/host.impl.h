
#pragma once

#include "mewt/emu/host/host.h"
#include "mewt/types/colour.h"
#include "mewt/types/span_2d.h"

namespace mewt::emu
{

	struct IHost::Frame
	{
		using PixelStore = types::Span2D<types::Colour, gfx::Image::Width, gfx::Image::Height>;
		PixelStore _pixels;
		class scan_out_t;
	};

	class IHost::Frame::scan_out_t
	{
	public:
		explicit scan_out_t(const Frame& frame) : _frame(frame) {}
		// inline auto operator*() const -> auto& { return *_pixel; }
		// inline auto operator++() -> auto&
		//{
		//	++_pixel;
		//	return *this;
		// }
		// inline void nextScanline()
		//{
		//	++_row;
		//	_pixel = std::addressof(*(*_row).begin());
		// }
		[[nodiscard]] inline auto isEndOfFrame() const -> bool { return _row == end(_frame._pixels.rows()); }

	private:
		const Frame& _frame;
		decltype(begin(_frame._pixels.rows())) _row = begin(_frame._pixels.rows());
		// types::Colour* _pixel = std::addressof(*(*_row).begin());
	};
}
