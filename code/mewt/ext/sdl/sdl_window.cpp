
#include "mewt/ext/sdl/sdl_window.h"
#include "mewt/ext/sdl/sdl_error.impl.h"

#include <SDL/SDL_video.h>

namespace mewt::ext::sdl
{

	template <>
	void sdl_destroyer<SDL_Window>::destroy(SDL_Window* object)
	{
		SDL_DestroyWindow(object);
	}

	window_t::window_t(std::string_view title, x_position_t x, y_position_t y, width_t w, height_t h, flags_t flags)
		 : super_t(checkPointer(SDL_CreateWindow(title.data(), x.get(), y.get(), w.get(), h.get(), flags.rawBits())))
	{
	}

	window_t::centered_t::operator window_t::x_position_t() const { return x_position_t(SDL_WINDOWPOS_CENTERED); }
	window_t::centered_t::operator window_t::y_position_t() const { return y_position_t(SDL_WINDOWPOS_CENTERED); }
	window_t::position_undefined_t::operator window_t::x_position_t() const { return x_position_t(SDL_WINDOWPOS_UNDEFINED); }
	window_t::position_undefined_t::operator window_t::y_position_t() const { return y_position_t(SDL_WINDOWPOS_UNDEFINED); }

}
