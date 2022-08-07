
#pragma once

#include <memory>

namespace mewt::ext::sdl {

	template <typename _Type>
	struct sdl_destroyer {
		void destroy(_Type* object);
		inline void operator()(_Type* object) { destroy(object); }
	};

	template <typename _Type>
	struct sdl_pointer : public std::unique_ptr<_Type, sdl_destroyer<_Type>> {
		using super_t = std::unique_ptr<_Type, sdl_destroyer<_Type>>;
		using super_t::super_t;
	};

}
