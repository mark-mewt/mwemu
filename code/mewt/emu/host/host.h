
#pragma once

#include "mewt/async/event.h"
#include "mewt/gfx/image.h"
#include "mewt/types/colour.h"
#include "mewt/types/span_2d.h"

namespace mewt::emu {

	class IHost
	{
	public:
		struct ConfigT
		{
			gfx::Image::Size display_size;
		};

		struct FrameT
		{
			using PixelStoreT = types::Span2dT<types::ColourT, gfx::Image::Width, gfx::Image::Height>;
			PixelStoreT _pixels;
			struct scan_out_t;
		};

		struct EventsT
		{
			async::EventT<ConfigT> initialising;
			async::EventT<FrameT> need_frame;
		};
		EventsT events;

		void runEmuHost();

		virtual ~IHost() = default;

	protected:
		virtual void initHost() = 0;

		ConfigT _host_config;
	};

	struct IHost::FrameT::scan_out_t
	{
		const FrameT& _frame;
		decltype(_frame._pixels.rows().begin()) _row = _frame._pixels.rows().begin();
		types::ColourT* _pixel = std::addressof(*(*_row).begin());
		explicit scan_out_t(const FrameT& frame) : _frame(frame) {}
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
