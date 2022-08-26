
#pragma once

#include "mewt/async/event_dispatch.h"
#include "mewt/async/future_promise.decl.h"

union SDL_Event;

namespace mewt::ext::sdl {

	class event_manager_t;

	class event_t {
	public:
		inline event_t(const SDL_Event& sdl_event) : _sdl_event(sdl_event) {}

	protected:
		int sdl_event_type() const;
		const SDL_Event& _sdl_event;
	};

	namespace keyboard {
		enum class scancode_t;
	}

	class keyboard_event_t : public event_t {
		using event_t::event_t;
	public:
		enum class event_type_t {
			KeyDown = 0x300,
			KeyUp,
		};
		inline auto event_type() const { return static_cast<event_type_t>(sdl_event_type()); }
		keyboard::scancode_t scancode() const;
		// We also have access to this:
		//Uint32 timestamp; /**< In milliseconds, populated using SDL_GetTicks() */
		//Uint32 windowID;	/**< The window with keyboard focus, if any */
		//Uint8 state;		/**< ::SDL_PRESSED or ::SDL_RELEASED */
		//Uint8 repeat;		/**< Non-zero if this is a key repeat */
		//SDL_Scancode scancode; /**< SDL physical key code - see ::SDL_Scancode for details */
		//SDL_Keycode sym;		  /**< SDL virtual key code - see ::SDL_Keycode for details */
		//Uint16 mod;				  /**< current key modifiers */
	};

	class event_manager_t {
	public:
		enum class event_type_t {
			Keyboard,
			Joystick,
		};
		template <event_type_t _Event>
		struct event_data_type;

		template <>
		struct event_data_type<event_type_t::Keyboard> { using type = keyboard_event_t; };

		using event_dispatch_t = async::event_dispatch_t<SDL_Event, event_type_t, event_data_type>;
		event_dispatch_t _event_dispatch;
		async::future<> pump();
		auto keyboard_event() { return event_dispatch_t::single_handler_t<event_type_t::Keyboard>(_event_dispatch); }
	};

}
