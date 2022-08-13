
#pragma once

union SDL_Event;

namespace mewt::ext::sdl {

	class main_loop_t;	// todo: inc. .decl.h
	class event_dispatch_t;

	namespace events {

		class base_event_t {
		protected:
			SDL_Event& _sdl_event;

		private:
			inline base_event_t(SDL_Event& sdl_event) : _sdl_event(sdl_event) {}
			friend event_dispatch_t;
		};

		struct keyboard_event_t : public base_event_t {
			using base_event_t::base_event_t;
		};

	}

	class event_dispatch_t {

	public:
	private:
		void dispatch_sdl(SDL_Event& event) const;
		template<typename _Event>
		void dispatch_event(const _Event& event) const;
		friend main_loop_t;
	};

	template<typename _Event>
	class event_handler_t {
	};

}
