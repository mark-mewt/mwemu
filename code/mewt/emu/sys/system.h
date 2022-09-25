
#pragma once

#include <mewt/types/interface.h>

namespace mewt::emu
{

	class IHost;

	struct ISystem : public types::IInterface
	{

		virtual void initSys(IHost& host) = 0;
	};

}