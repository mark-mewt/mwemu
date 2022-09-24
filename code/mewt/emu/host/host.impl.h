
#pragma once

#include "mewt/emu/host/host.h"
#include "mewt/types/colour.h"
#include "mewt/types/span_2d.h"

namespace mewt::emu
{

	struct IHost::Frame
	{
		using PixelStore = types::Span2dT<types::Colour, gfx::Image::Width, gfx::Image::Height>;
		PixelStore _pixels;
		struct scan_out_t;
	};

	struct IHost::Frame::scan_out_t
	{
		const Frame& _frame;
		decltype(_frame._pixels.rows().begin()) _row = _frame._pixels.rows().begin();
		types::Colour* _pixel = std::addressof(*(*_row).begin());
		explicit scan_out_t(const Frame& frame) : _frame(frame) {}
		inline auto operator*() const -> auto& { return *_pixel; }
		inline auto operator++() -> auto&
		{
			++_pixel;
			return *this;
		}
		inline void nextScanline()
		{
			++_row;
			_pixel = std::addressof(*(*_row).begin());
		}
		[[nodiscard]] inline auto isEndOfFrame() const -> bool { return _row == _frame._pixels.rows().end(); }
	};
}
