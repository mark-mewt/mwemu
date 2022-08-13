
#pragma once

#include "mewt/ext/sdl/sdl_event_dispatch.h"

namespace mewt::ext::sdl {

	class main_loop_t {
		class iterator_t;
		class sentinel_t;

	public:

		main_loop_t(event_dispatch_t& event_dispatch);

		iterator_t begin();
		sentinel_t end();

		bool is_active() const;

	private:
		void frame_advance();
		class frame_controls_t {
		public:
		private:
			main_loop_t& _main_loop;
			frame_controls_t(main_loop_t& main_loop) : _main_loop(main_loop) {}
			friend main_loop_t;
		};
		class sentinel_t {};
		class iterator_t {
		public:
			inline friend bool operator!=(iterator_t lhs, sentinel_t rhs) { return lhs._main_loop.is_active(); }
			inline iterator_t operator++() {
				_main_loop.frame_advance();
				return *this;
			}
			inline frame_controls_t& operator*() const { return _main_loop._frame_controls; }
		private:
			main_loop_t& _main_loop;
			inline iterator_t(main_loop_t& main_loop) : _main_loop(main_loop) {}
			friend main_loop_t;
		};
		frame_controls_t _frame_controls{ *this };
		event_dispatch_t& _event_dispatch;
		bool _is_active = true; 
	};

}