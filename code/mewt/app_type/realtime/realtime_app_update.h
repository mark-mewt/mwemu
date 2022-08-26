
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/types/opaque.h"

namespace mewt::app_type::realtime {

	class realtime_app_t::update_state_t {
		enum class tags_t {
			FrameIndex
		};
		constexpr friend auto get_opaque_value_type(tags_t) -> std::uint32_t;

	public:
		types::opaque<tags_t::FrameIndex> _frameIndex;
	};


}
