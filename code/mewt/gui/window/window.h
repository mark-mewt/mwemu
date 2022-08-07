
#pragma once

#include "mewt/os/app_context/app_context.decl.h"
#include "mewt/gui/window_manager/window_manager.decl.h"

#include <memory>

namespace mewt::gui {

	struct window_i;

	using window_t = std::unique_ptr<window_i>;

	struct window_i {

		static window_t create(window_manager_t& window_manager);

		virtual void run() = 0;

		inline virtual ~window_i() = default;

	};

}
