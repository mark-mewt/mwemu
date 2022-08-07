
#pragma once

#include "mewt/ext/sdl/sdl_types.h"

#include "mewt/types/opaque.h"

struct SDL_PixelFormat;

namespace mewt::ext::sdl {

	class pixel_format_t : public sdl_pointer<SDL_PixelFormat> {
		enum class tag_t {
			Coded,
		};
		enum class sdl_format;
		constexpr friend auto get_opaque_value_type(tag_t) -> sdl_format;

	public:
		enum class preset_t {
			ARGB8888
		};
		using coded_t = types::opaque<tag_t::Coded>;
		template <preset_t _Preset>
		inline static coded_t coded() { return preset_map_t<_Preset>::get_coded(); }

	private:
		template <preset_t _Preset>
		struct preset_map_t {
			static coded_t get_coded();
		};
	};

}
