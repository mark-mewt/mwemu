
#pragma once

#include "mewt/ext/sdl/sdl_image.h"

#include "SDL/SDL_rect.h"

#include <optional>

namespace mewt::ext::sdl
{

	static auto proxy_sdl_rect(const std::optional<image_t::Rect>& rect)
	{
		struct proxy_t
		{
			const std::optional<image_t::Rect>& _rect;
			SDL_Rect _sdl_rect;
			inline operator SDL_Rect*() &&
			{
				if (!_rect.has_value())
					return nullptr;
				_sdl_rect.x = left(*_rect).get();
				_sdl_rect.y = top(*_rect).get();
				_sdl_rect.w = _rect->size._width.get();
				_sdl_rect.h = _rect->size._height.get();
				return &_sdl_rect;
			}
		};
		return proxy_t{ rect };
	}

}
