
#pragma once

#include "mewt/ext/sdl/sdl_renderer.decl.h"

#include "mewt/ext/sdl/sdl_types.h"
#include "mewt/types/opaque.h"
#include "mewt/types/flags.h"
#include "mewt/ext/sdl/sdl_image.h"

#include "mewt/ext/sdl/sdl_window.decl.h"
#include "mewt/ext/sdl/sdl_texture.decl.h"

#include <optional>

struct SDL_Renderer;

namespace mewt::ext::sdl {

	class renderer_t : public sdl_pointer<SDL_Renderer> {
		using super_t = sdl_pointer<SDL_Renderer>;
		enum tags_t {
			DriverIndex
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using driver_index_t = types::opaque<tags_t::DriverIndex>;
		static constexpr driver_index_t FirstDriver{ -1 };
		enum class flag_t {
			Software = 0,
			Accelerated = 1,
			PresentVSync = 2,
			TargetTexture = 3,
		};
		using flags_t = types::flags<flag_t>;
		renderer_t(const window_t& window, driver_index_t driver_index, flags_t flags);
		struct copy_args_t {
			std::optional<image_t::rect_t> _src;
			std::optional<image_t::rect_t> _dest;
		};
		void copy(const texture_t& texture, copy_args_t copy_args) const;
		image_t::rect_t get_output_bounds() const;
		void clear_screen() const;
		void present_screen() const;
	};

}
