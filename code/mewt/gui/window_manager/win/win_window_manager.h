
#pragma once

#include "mewt/gui/window_manager/win/win_window_manager.decl.h"
#include "mewt/gui/window_manager/window_manager.h"
#include "mewt/os/app_context/win/win_app_context.h"

namespace mewt::gui::win {

	class window_manager_t : public window_manager_i {

	public:
		window_manager_t(const os::win::app_context_t& app_context);

		inline auto window_class_atom() const { return _window_class_atom; }
		inline auto& app_context() const { return _app_context; }

	private:
		const os::win::app_context_t& _app_context;
		::ATOM _window_class_atom;

	};

}
