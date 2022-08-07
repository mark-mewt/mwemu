
#pragma once

#include <memory>

namespace mewt::gui {

	struct window_manager_i;

	using window_manager_t = std::unique_ptr<window_manager_i>;

}
