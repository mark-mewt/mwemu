
#pragma once

#include "mewt/gui/window_manager/window_manager.decl.h"
#include "mewt/os/app_context/app_context.decl.h"

namespace mewt::gui {

	struct window_manager_i {

		static window_manager_t create(const os::app_context_i& app_context);

		inline virtual ~window_manager_i() = default;

	};

}
