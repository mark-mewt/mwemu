
#pragma once

#include "mewt/app_type/realtime/realtime_app.h"
#include "mewt/types/opaque.h"

namespace mewt::app_type::realtime {

	class realtime_app_t::update_state_t {
		enum class tags_t {
			FrameIndex
		};
		constexpr friend auto getOpaqueValueType(tags_t) -> std::uint32_t;

	public:
		types::Opaque<tags_t::FrameIndex> _frameIndex;
	};


}
