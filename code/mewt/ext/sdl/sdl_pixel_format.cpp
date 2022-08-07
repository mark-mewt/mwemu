
#include "mewt/ext/sdl/sdl_pixel_format.h"

#include "SDL/SDL_pixels.h"

namespace mewt::ext::sdl {

	template <>
	pixel_format_t::coded_t pixel_format_t::preset_map_t<pixel_format_t::preset_t::ARGB8888>::get_coded() { return coded_t(static_cast<sdl_format>(SDL_PIXELFORMAT_ARGB8888)); }

}
