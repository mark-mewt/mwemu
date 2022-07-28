
#include "mewt/io/reader.h"

#include <iostream>
#include <fstream>

namespace mewt::io {

	void reader_t::read_data(const std::string_view& name, void* data, size_t size, size_t offset) {
		std::ifstream file(name.data(), std::ios::binary | std::ios::in);
		if (file.fail())
			throw std::exception("bad file");
		file.seekg(offset);
		file.read((char*)data, size);
	}

}
