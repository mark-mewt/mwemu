
#include "mewt/ext/sdl/sdl_texture.h"

#include "mewt/ext/sdl/sdl_renderer.h"

#include "mewt/ext/sdl/sdl_error.impl.h"

#include "SDL/SDL_render.h"

namespace mewt::ext::sdl {

	template <>
	void sdl_destroyer<SDL_Texture>::destroy(SDL_Texture* object) {
		SDL_DestroyTexture(object);
	}

	texture_t::texture_t(const renderer_t& renderer, const texture_config_t& config)
		 : super_t(check_pointer(SDL_CreateTexture(renderer.get(), (int)config._format.get(), (int)config._access, config._size._width.get(), config._size._height.get()))) {
	}

	texture_config_t texture_t::get_config() const {
		Uint32 format;
		int access;
		int width;
		int height;
		check_status(SDL_QueryTexture(get(), &format, &access, &width, &height));
		return texture_config_t{
			._format = pixel_format_t::coded_t(static_cast<std::decay_t<decltype(types::dummyArg<pixel_format_t::coded_t>().get())>>(format)),
			._access = static_cast<texture_config_t::access_t>(access),
			._size = {
				 ._width = image_t::Width(width),
				 ._height = image_t::Height(height) }
		};
	}

}
