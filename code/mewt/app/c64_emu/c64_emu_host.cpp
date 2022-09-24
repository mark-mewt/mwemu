
#include "mewt/app/c64_emu/c64_emu_host.h"
#include "mewt/app_type/realtime/realtime_app.impl.h"
#include "mewt/app_type/realtime/realtime_app_phase.impl.h"
#include "mewt/app_type/realtime/realtime_app_state.h"
#include "mewt/async/future_promise.h"
#include "mewt/emu/sys/c64/c64_ntsc.h"
#include "mewt/emu/sys/c64/c64_pal.h"
#include "mewt/ext/sdl/sdl_scancode.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/types/scale_factor.h"
#include "mewt/emu/host/host.impl.h"
#include "mewt/ext/sdl/sdl_event_manager.impl.h"
#include "mewt/async/event.impl.h"

// mwToDo: Abstract these away
#include "SDL/SDL_render.h"

namespace mewt::app::c64_emu {

	EmulatorHost::EmulatorHost(app_type::realtime::realtime_app_t& app)
		 : _app(app) {
	}

	EmulatorHost::~EmulatorHost() = default;

	void EmulatorHost::initHost()
	{
		runUpdater();
		runInput();
		runRenderer();
		const bool use_pal = true;
		if (use_pal)
			_c64 = std::make_unique<emu::sys::c64::c64_pal_t>();
		else
			_c64 = std::make_unique<emu::sys::c64::c64_ntsc_t>();
		_c64->init_sys(*this);
	}

	auto EmulatorHost::runRenderer()
		-> async::Future<>
	{

		const auto& init_state = co_await _app.initPhase();

		using ext::sdl::texture_config_t;
		using ext::sdl::pixel_format_t;
		using ext::sdl::texture_t;
		texture_config_t texture_config{
			._format = pixel_format_t::coded<pixel_format_t::preset_t::ARGB8888>(),
			._access = texture_config_t::access_t::Streaming,
			._size = _host_config.display_size
		};

		texture_t sdl_texture(init_state.renderer(), texture_config);

		auto output_bounds = init_state.renderer().get_output_bounds();

		for (;;) {
			co_await _app.updatePhase();
			void* pixels_void = nullptr;
			int pitch = 0;
			SDL_LockTexture(sdl_texture.get(), nullptr, &pixels_void, &pitch);
			auto* pixels = static_cast<types::Colour*>(pixels_void);
			auto span = types::Span2dT(pixels, _host_config.display_size._width, _host_config.display_size._height, gfx::Image::Width(pitch / static_cast<int>(sizeof(types::Colour))));
			//static int kk = 0;
			//kk++;
			//for (int i = 0; i < 32 * 32; ++i)
			//	pixels[i] = i + kk;
			Frame frame{ ._pixels = span };
			events.need_frame.dispatch(frame);
			SDL_UnlockTexture(sdl_texture.get());
			const auto& render_data = co_await _app.renderPhase();
			render_data.renderer().copy(sdl_texture, { ._src = std::nullopt, ._dest = output_bounds });
		}
	}

	auto EmulatorHost::runInput()
		-> async::Future<>
	{
		for (;;) {
			auto input_event = co_await _app.eventManager().keyboard_event();
			switch (input_event.event_type()) {
			case ext::sdl::KeyboardEvent::EventType::KeyDown:
			case ext::sdl::KeyboardEvent::EventType::KeyUp:
				break;
			}
		}
	}

	auto EmulatorHost::runUpdater()
		 -> async::Future<>
	{
		for (;;) {
			co_await _app.updatePhase();
		}
	}

}
