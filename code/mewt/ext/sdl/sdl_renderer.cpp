
#include "mewt/ext/sdl/sdl_error.impl.h"
#include "mewt/ext/sdl/sdl_image.impl.h"
#include "mewt/ext/sdl/sdl_renderer.h"
#include "mewt/ext/sdl/sdl_texture.h"
#include "mewt/ext/sdl/sdl_window.h"

#include <SDL/SDL_render.h>

namespace mewt::ext::sdl
{

	template <>
	void sdl_destroyer<SDL_Renderer>::destroy(SDL_Renderer* object)
	{
		SDL_DestroyRenderer(object);
	}

	renderer_t::renderer_t(const window_t& window, driver_index_t driver_index, flags_t flags)
		 : super_t(checkPointer(SDL_CreateRenderer(window.get(), driver_index.get(), flags.rawBits())))
	{
	}

	void renderer_t::copy(const texture_t& texture, copy_args_t copy_args) const
	{
		checkStatus(SDL_RenderCopy(get(), texture.get(), proxy_sdl_rect(copy_args._src), proxy_sdl_rect(copy_args._dest)));
	}

	image_t::Rect renderer_t::get_output_bounds() const
	{
		int width, height;
		checkStatus(SDL_GetRendererOutputSize(get(), &width, &height));
		return image_t::Rect{
			._position = {
				 ._x = image_t::XPosition(0),
				 ._y = image_t::YPosition(0) },
			.size = { ._width = image_t::Width(width), ._height = image_t::Height(height) }
		};
	}

	void renderer_t::clear_screen() const
	{
		checkStatus(SDL_RenderClear(get()));
	}

	void renderer_t::present_screen() const
	{
		SDL_RenderPresent(get());
	}

}
