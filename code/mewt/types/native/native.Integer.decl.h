
#pragma once

#include "mewt/types/mewt.types.decl.h"

#include <cstdint>

namespace mewt::types
{

	namespace native
	{

		enum class Byte : std::uint8_t;
		enum class Word : std::uint16_t;
		enum class DWord : std::uint32_t;
		enum class QWord : std::uint64_t;

	}

	using namespace native;

}