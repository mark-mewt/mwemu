
#include "mewt/emu/host/host.h"
#include "mewt/async/event.impl.h"

namespace mewt::emu
{

	void IHost::runEmuHost()
	{
		initHost();
		events().initialising.dispatch(_host_config);
	}

}
