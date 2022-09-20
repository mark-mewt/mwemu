
#include "mewt/emu/host/host.h"

namespace mewt::emu {

	void IHost::runEmuHost()
	{
		initHost();
		events.initialising.dispatch(_host_config);
	}

}
