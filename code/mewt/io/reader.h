
#pragma once

#include <string>

namespace mewt::io
{

	class reader_t
	{

	public:

		void read_data(const std::string_view& name, void* data, size_t size, size_t offset = 0);

	};

}
