
#pragma once

#include "mewt/gui/window/window.h"
#include "mewt/gui/window_manager/win/win_window_manager.decl.h"
#include "mewt/os/base/win_base.h"

namespace mewt::gui::win {

	class window_t : public window_i {

	public:
		window_t(window_manager_t& window_manager);

		void run() override final;

	private:
		window_manager_t& _window_manager;
		::HWND _hwnd;

	};

}
