
#pragma once

#include <concepts>

namespace mewt::emu::device
{

	template<typename TDevice>
	struct Info
	{
	
		//constexpr static auto kFoo;
		using Device = TDevice;

	};

	template<typename TDeviceInfo>
	concept isDeviceInfo = std::derived_from<TDeviceInfo, Info<typename TDeviceInfo::Device>>;
}
