
#include "mewt/io/reader.h"

#include <fstream>
#include <iostream>

namespace mewt::io
{

	void reader_t::read_data(const std::string_view& name, std::span<types::Byte> target, size_t offset)
	{
		std::ifstream file(name.data(), std::ios::binary | std::ios::in);
		if (file.fail())
			throw std::exception("bad file");
		file.seekg(offset);
		file.read((char*)target.data(), target.size_bytes());
	}

}
