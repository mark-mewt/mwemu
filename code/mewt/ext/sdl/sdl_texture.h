
#pragma once

#include "mewt/ext/sdl/sdl_texture.decl.h"

#include "mewt/ext/sdl/sdl_renderer.decl.h"

#include "mewt/ext/sdl/sdl_types.h"
#include "mewt/ext/sdl/sdl_image.h"
#include "mewt/ext/sdl/sdl_pixel_format.h"

struct SDL_Texture;

namespace mewt::ext::sdl {

	struct texture_config_t {
		enum class access_t {
			Static,
			Streaming,
			Target,
		};
		pixel_format_t::coded_t _format;
		access_t _access;
		image_t::Size _size;
	};

	class texture_t : public sdl_pointer<SDL_Texture> {
		using super_t = sdl_pointer<SDL_Texture>;

	public:
		texture_t(const renderer_t& renderer, const texture_config_t& config);

		texture_config_t get_config() const;
	};

}
