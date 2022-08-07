
#pragma once

#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_timer.h"

#include "mewt/types/flags.h"
#include "mewt/types/non_movable.h"
#include "mewt/types/arithmetic.h"

#include <memory>
#include <string>
#include <optional>

namespace mewt::ext::sdl {

	struct exception_t {
		const char* _msg = nullptr;
	};

	enum class subsystem_t {
		Timer = 0,
		Audio = 4,
		Video = 5,
		Joystick = 9,
		Haptic = 12,
		GameController = 13,
		Events = 14,
		Sensor = 15,
	};

	using subsystems_t = types::flags<subsystem_t>;
	using types::operator|;

	class engine_t : types::non_movable_t {

	public:

		explicit engine_t(subsystems_t subsystems);

		void start_subsystems(subsystems_t subsystems);
		void stop_subsystems(subsystems_t subsystems);

		subsystems_t query_started_subsystems(subsystems_t query_mask = types::no_flags_t{});

		~engine_t();

	};

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

	template <auto _Tag>//, typename _Type, typename _TagType = decltype(_Tag)>
	class opaque {
	public:
		using tag_type_t = decltype(_Tag);
		using value_type_t = decltype(get_opaque_value_type(_Tag));
		template <typename... _Args>
		constexpr explicit opaque(_Args&&... args) : _value(std::forward<_Args>(args)...) { }
		constexpr explicit operator const value_type_t&() const { return _value; }
		constexpr const value_type_t& get() const { return _value; }
		constexpr friend auto operator<=>(opaque lhs, opaque rhs) {
			return lhs._value <=> rhs._value;
		}
	private:
		value_type_t _value;
	};

	template <auto _Tag>
	constexpr auto operator-(opaque<_Tag> lhs, opaque<_Tag> rhs) {
		constexpr auto diff = get_opaque_difference(_Tag);
		return opaque<diff>(lhs.get() - rhs.get());
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto& operator-=(opaque<_TagL>& lhs, opaque<_TagR> rhs) {
		lhs = opaque<_TagL>(lhs.get() - rhs.get());
		return lhs;
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) constexpr auto& operator+=(opaque<_TagL>& lhs, opaque<_TagR> rhs) {
		lhs = opaque<_TagL>(lhs.get() + rhs.get());
		return lhs;
	}

	// template <auto _Tag>
	//constexpr auto& operator-=(opaque<_Tag>& lhs, opaque<get_opaque_difference(_Tag)> rhs) {
	//	lhs = opaque<_Tag>(lhs.get() - rhs.get());
	//	return lhs;
	//}

	//template <auto _Tag>
	//constexpr auto& operator+=(opaque<_Tag>& lhs, opaque<get_opaque_difference(_Tag)> rhs) {
	//	lhs = opaque<_Tag>(lhs.get() + rhs.get());
	//	return lhs;
	//}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) 
	constexpr auto operator-(opaque<_TagL> lhs, opaque<_TagR> rhs) {
		return opaque<_TagL>(lhs.get() - rhs.get());
	}

	template <auto _TagL, auto _TagR>
	requires(get_opaque_difference(_TagL) == _TagR) 
	constexpr auto operator+(opaque<_TagL> lhs, opaque<_TagR> rhs) {
		return opaque<_TagL>(lhs.get() + rhs.get());
	}

	// template <auto _Tag, types::IS_arithmetic _Type>
	//constexpr auto get_type_traits(types::class_id<opaque<_Tag, _Type>>)
	//	-> types::class_id<types::arithmetic_traits<opaque<_Tag, _Type>>>;

	/* template <auto _TagL, types::IS_arithmetic _TypeL, auto _TagR, types::IS_arithmetic _TypeR>
	constexpr auto& operator-=(opaque<_TagL, _TypeL>&& lhs, const opaque<_TagR, _TypeR>& rhs) {
		///types::type_tester<decltype(opaque_difference())> t0;
		types::type_tester<_TypeL> t1;
		types::type_tester<decltype(_TagR)> t2;
		types::type_tester<_TypeR> t3;
	}*/

	class scale_factor_t {
		enum class tags_t {
			Numerator,
			Denominator
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;
	public:
		using numerator_t = opaque<tags_t::Numerator>;//, int>;
		using denominator_t = opaque<tags_t::Denominator>;//, int>;
		static constexpr scale_factor_t Half() { return { numerator_t(1), denominator_t(2) }; };
		template <auto _Tag>
		requires(get_opaque_difference(_Tag) == _Tag) constexpr auto scale(opaque<_Tag> v) const { return opaque<_Tag>((v.get() * _numerator.get()) / _denominator.get()); }
	private:
		constexpr scale_factor_t(numerator_t numerator, denominator_t denominator)
			 : _numerator(numerator), _denominator(denominator) {
		}
		numerator_t _numerator;
		denominator_t _denominator;
	};

	//template <auto _Tag, typename _Type>
	//constexpr auto operator-(opaque<_Tag, _Type> lhs, opaque<_Tag, _Type> rhs) { return opaque<_Tag, _Type>(lhs.get() - rhs.get()); }
	//template <auto _Tag, typename _Type>
	//constexpr auto operator*(opaque<_Tag, _Type> lhs, scale_factor_t rhs) { return opaque<_Tag, _Type>(rhs.scale(lhs.get())); }
	//template <auto _Tag, typename _Type>
	//constexpr auto operator*(scale_factor_t lhs, opaque<_Tag, _Type> rhs) { return opaque<_Tag, _Type>(lhs.scale(rhs.get())); }

	class window_t : public sdl_pointer<SDL_Window> {
		using super_t = sdl_pointer<SDL_Window>;
		enum class tags_t {
			XPosition,
			YPosition,
			Width,
			Height
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using x_position_t = opaque<tags_t::XPosition>;//, int>;
		using y_position_t = opaque<tags_t::YPosition>;//, int>;
		using width_t = opaque<tags_t::Width>;//, int>;
		using height_t = opaque<tags_t::Height>;//, int>;
		struct centered_t {
			operator x_position_t() const;
			operator y_position_t() const;
		};
		struct position_undefined_t {
			operator x_position_t() const;
			operator y_position_t() const;
		};

		enum class flag_t {
			FullScreen = 0,
			FullScreenDesktop = 12,
			OpenGL = 1,
			Vulkan = 28,
			Metal = 29,
			Hidden = 3,
			Borderless = 4,
			Resizable = 5,
			Minimized = 6,
			Maximized = 7,
			InputGrabbed = 8,
			AllowHighDPI = 13,
		};

		using flags_t = types::flags<flag_t>;

		window_t(std::string_view title, x_position_t x, y_position_t y, width_t w, height_t h, flags_t flags);

	};

	class image_t {
		enum class tags_t {
			X,
			Y,
			Width,
			Height
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using x_position_t = opaque<tags_t::X>;//, int>;
		using y_position_t = opaque<tags_t::Y>;//, int>;
		using width_t = opaque<tags_t::Width>;//, int>;
		using height_t = opaque<tags_t::Height>;//, int>;
		constexpr friend auto get_opaque_difference(tags_t tag) {
			switch (tag) {
			case tags_t::X:
				return tags_t::Width;
			case tags_t::Y:
				return tags_t::Height;
			case tags_t::Width:
				return tags_t::Width;
			case tags_t::Height:
				return tags_t::Height;
			}
		}
		//static_assert(types::IS_custom_arithmetic<width_t>);
		struct position_t {
			x_position_t _x;
			y_position_t _y;
		};
		struct size_t {
			width_t _width;
			height_t _height;
			constexpr static void with_components(auto fn) {
				fn(&size_t::_width);
				fn(&size_t::_height);
			}
		};
		struct rect_t;
	};
	struct image_t::rect_t {
		position_t _position;
		size_t _size;
		constexpr auto& left() const { return _position._x; }
		constexpr auto right() const { return _position._x + _size._width; } // todo: add proxy to non-const version so we can assign to it
		constexpr auto& top() const { return _position._y; }
		constexpr auto bottom() const { return _position._y + _size._height; }
	};
	/* constexpr auto operator-(image_t::size_t lhs, image_t::size_t rhs) {
		auto x = lhs._width - rhs._width;
		return image_t::size_t{
			._width = lhs._width - rhs._width,// lhs._width - rhs._width,
			._height = lhs._height - rhs._height
		};
	}*/

	class texture_t;

	class renderer_t : public sdl_pointer<SDL_Renderer> {
		using super_t = sdl_pointer<SDL_Renderer>;
		enum tags_t {
			DriverIndex
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> int;

	public:
		using driver_index_t = opaque<tags_t::DriverIndex>;//, int>;
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
	};

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
		using coded_t = opaque<tag_t::Coded>;//, sdl_format>;
		template<preset_t _Preset>
		inline static coded_t coded() { return preset_map_t<_Preset>::get_coded(); }

	private:
		template<preset_t _Preset>
		struct preset_map_t {
			static coded_t get_coded();
		};
	};

	struct texture_config_t {
		enum class access_t {
			Static,
			Streaming,
			Target,
		};
		pixel_format_t::coded_t _format;
		access_t _access;
		image_t::size_t _size;
	};

	class texture_t : public sdl_pointer<SDL_Texture> {
		using super_t = sdl_pointer<SDL_Texture>;

	public:
		texture_t(const renderer_t& renderer, const texture_config_t& config);

		texture_config_t get_config() const;

	};

}
