
#pragma once

namespace mewt::types
{

	struct NonMovable
	{
		constexpr NonMovable() = default;
		constexpr NonMovable(const NonMovable&) = delete;
		constexpr NonMovable(NonMovable&&) = delete;
		constexpr auto operator=(const NonMovable&) = delete;
		constexpr auto operator=(NonMovable&&) = delete;
		inline ~NonMovable() = default;
	};

}
