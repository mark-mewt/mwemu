
#pragma once

#include "mewt/ext/sdl/sdl_window.decl.h"

#include "mewt/ext/sdl/sdl_types.h"
#include "mewt/types/opaque.h"
#include "mewt/types/flags.h"

#include <string>

struct SDL_Window;

namespace mewt::ext::sdl {

	class window_t : public sdl_pointer<SDL_Window> {
		using super_t = sdl_pointer<SDL_Window>;
		enum class tags_t {
			XPosition,
			YPosition,
			Width,
			Height
		};
		constexpr friend auto getOpaqueValueType(tags_t) -> int;

	public:
		using x_position_t = types::Opaque<tags_t::XPosition>;
		using y_position_t = types::Opaque<tags_t::YPosition>;
		using width_t = types::Opaque<tags_t::Width>;
		using height_t = types::Opaque<tags_t::Height>;
		struct centered_t {
			operator x_position_t() const;
			operator y_position_t() const;
		};
		struct position_undefined_t {
			operator x_position_t() const;
			operator y_position_t() const;
		};

		enum class flag_t {
			FullScreen = 0,
			FullScreenDesktop = 12,
			OpenGL = 1,
			Vulkan = 28,
			Metal = 29,
			Hidden = 3,
			Borderless = 4,
			Resizable = 5,
			Minimized = 6,
			Maximized = 7,
			InputGrabbed = 8,
			AllowHighDPI = 13,
		};

		using flags_t = types::flags<flag_t>;

		window_t(std::string_view title, x_position_t x, y_position_t y, width_t w, height_t h, flags_t flags);
	};

}
