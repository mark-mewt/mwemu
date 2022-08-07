
#include "mewt/ext/sdl/sdl.h"
#include "mewt/diag/log.h"

namespace mewt::ext::sdl {

	static void check_status(int status, std::source_location loc = std::source_location::current()) {
		if (status != 0) {
			auto* err = SDL_GetError();
			logger(loc).log("SDL Error: %s", err);
			throw exception_t{ err };
		}
	}

	static auto* check_pointer(auto* object, std::source_location loc = std::source_location::current()) {
		if (object == nullptr) {
			auto* err = SDL_GetError();
			logger(loc).log("SDL Error: %s", err);
			throw exception_t{ err };
		}
		return object;
	}

	engine_t::engine_t(subsystems_t subsystems) {
		check_status(SDL_InitSubSystem(subsystems.raw_bits()));
	}

	void engine_t::start_subsystems(subsystems_t subsystems) {
		check_status(SDL_InitSubSystem(subsystems.raw_bits()));
	}

	void engine_t::stop_subsystems(subsystems_t subsystems) {
		SDL_QuitSubSystem(subsystems.raw_bits());
	}

	subsystems_t engine_t::query_started_subsystems(subsystems_t query_mask) {
		return subsystems_t(SDL_WasInit(query_mask.raw_bits()));
	}

	engine_t::~engine_t() {
		SDL_Quit();
	}

	template<>
	void sdl_destroyer<SDL_Window>::destroy(SDL_Window* object) {
		SDL_DestroyWindow(object);
	}

	window_t::window_t(std::string_view title, x_position_t x, y_position_t y, width_t w, height_t h, flags_t flags)
		 : super_t(check_pointer(SDL_CreateWindow(title.data(), x.get(), y.get(), w.get(), h.get(), flags.raw_bits()))) {
	}

	window_t::centered_t::operator window_t::x_position_t() const { return x_position_t(SDL_WINDOWPOS_CENTERED); }
	window_t::centered_t::operator window_t::y_position_t() const { return y_position_t(SDL_WINDOWPOS_CENTERED); }
	window_t::position_undefined_t::operator window_t::x_position_t() const { return x_position_t(SDL_WINDOWPOS_UNDEFINED); }
	window_t::position_undefined_t::operator window_t::y_position_t() const { return y_position_t(SDL_WINDOWPOS_UNDEFINED); }

	template <>
	void sdl_destroyer<SDL_Renderer>::destroy(SDL_Renderer* object) {
		SDL_DestroyRenderer(object);
	}

	renderer_t::renderer_t(const window_t& window, driver_index_t driver_index, flags_t flags)
		 : super_t(check_pointer(SDL_CreateRenderer(window.get(), driver_index.get(), flags.raw_bits()))) {
	}

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
			._format = pixel_format_t::coded_t(static_cast<std::decay_t<decltype(types::dummy_arg<pixel_format_t::coded_t>().get())>>(format)),
			._access = static_cast<texture_config_t::access_t>(access),
			._size = {
				 ._width = image_t::width_t(width),
				 ._height = image_t::height_t(height) }
		};
	}

	template<>
	pixel_format_t::coded_t pixel_format_t::preset_map_t<pixel_format_t::preset_t::ARGB8888>::get_coded() { return coded_t(static_cast<sdl_format>(SDL_PIXELFORMAT_ARGB8888)); }

	//static auto as_sdl_rect(const image_t::rect_t& rect) {
	//}

	static auto proxy_sdl_rect(const std::optional<image_t::rect_t>& rect) {
		struct proxy_t {
			const std::optional<image_t::rect_t>& _rect;
			SDL_Rect _sdl_rect;
			inline operator SDL_Rect*() && {
				if (!_rect.has_value())
					return nullptr;
				_sdl_rect.x = _rect->left().get();
				_sdl_rect.y = _rect->top().get();
				_sdl_rect.w = _rect->_size._width.get();
				_sdl_rect.h = _rect->_size._height.get();
				return &_sdl_rect;
			}
		};
		return proxy_t{ rect };
	}

	void renderer_t::copy(const texture_t& texture, copy_args_t copy_args) const {
		check_status(SDL_RenderCopy(get(), texture.get(), proxy_sdl_rect(copy_args._src), proxy_sdl_rect(copy_args._dest)));
	}

	image_t::rect_t renderer_t::get_output_bounds() const {
		int width, height;
		check_status(SDL_GetRendererOutputSize(get(), &width, &height));
		return image_t::rect_t{
			._position = {
				 ._x = image_t::x_position_t(0),
				 ._y = image_t::y_position_t(0) },
			._size = { ._width = image_t::width_t(width), ._height = image_t::height_t(height) }
		};
	}

}
