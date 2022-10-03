
#pragma once

#include "mewt/types/int_types.h"

#include <string>
#include <span>

namespace mewt::io
{

	class reader_t
	{

	public:
		static void read_data(const std::string_view& name, std::span<types::Byte> target, size_t offset = 0);
	};

}
