
#pragma once

#include "mewt/async/event_dispatch.h"
#include "mewt/async/future_promise.decl.h"

union SDL_Event;

namespace mewt::ext::sdl
{

	class EventManager;

	class Event
	{
	public:
		inline explicit Event(const SDL_Event& sdl_event) : _sdl_event(sdl_event) {}

	protected:
		[[nodiscard]] auto sdlEventType() const -> int;
		[[nodiscard]] inline auto sdlEvent() const -> const SDL_Event& { return _sdl_event; }

	private:
		const SDL_Event& _sdl_event;
	};

	namespace keyboard
	{
		enum class Scancode;
	}

	class KeyboardEvent : public Event
	{
		using Event::Event;

	public:
		enum class EventType
		{
			KeyDown = 0x300,
			KeyUp,
		};
		[[nodiscard]] inline auto eventType() const { return static_cast<EventType>(sdlEventType()); }
		[[nodiscard]] auto scancode() const -> keyboard::Scancode;
		// We also have access to this:
		// Uint32 timestamp; /**< In milliseconds, populated using SDL_GetTicks() */
		// Uint32 windowID;	/**< The window with keyboard focus, if any */
		// Uint8 state;		/**< ::SDL_PRESSED or ::SDL_RELEASED */
		// Uint8 repeat;		/**< Non-zero if this is a key repeat */
		// SDL_Scancode scancode; /**< SDL physical key code - see ::SDL_Scancode for details */
		// SDL_Keycode sym;		  /**< SDL virtual key code - see ::SDL_Keycode for details */
		// Uint16 mod;				  /**< current key modifiers */
	};

	class EventManager
	{
	public:
		enum class EventType
		{
			Keyboard,
			Joystick,
		};
		template <EventType NEvent>
		struct EventDataType;

		template <>
		struct EventDataType<EventType::Keyboard>
		{
			using Type = KeyboardEvent;
		};

		auto pump() -> async::Future<>;
		inline auto keyboardEvent();

	private:
		using EventDispatch = async::EventDispatch<SDL_Event, EventType, EventDataType>;
		EventDispatch _event_dispatch;
	};

}
