
#pragma once

namespace mewt::types
{

	struct IInterface
	{
		constexpr IInterface() = default;
		constexpr IInterface(const IInterface&) = delete;
		constexpr IInterface(IInterface&&) = delete;
		constexpr IInterface& operator=(const IInterface&) = delete;
		constexpr IInterface& operator=(IInterface&&) = delete;
		virtual ~IInterface() = default;
	};

}
