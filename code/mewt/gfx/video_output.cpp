
#include "mewt/gfx/video_output.h"

#include <exception>

namespace mewt::gfx::video_output
{

	template <Standard _Standard>
	struct standard_info;

	template <>
	struct standard_info<Standard::NtscM>
	{
		static constexpr Config config = {
			.refresh_rate_hz = 60,
		};
	};

	template <>
	struct standard_info<Standard::PalB>
	{
		static constexpr Config config = {
			.refresh_rate_hz = 50,
		};
	};

	Config Config::get(Standard standard)
	{
		switch (standard)
		{
		case Standard::NtscM:
			return standard_info<Standard::NtscM>::config;
		case Standard::PalB:
			return standard_info<Standard::PalB>::config;
		}
		throw std::exception("Unknown standard.");
	}

}
