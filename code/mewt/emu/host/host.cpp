
#include "mewt/emu/host/host.h"

namespace mewt::emu {

	void host_t::run_emu_host() {
		init_host();
		events.initialising.dispatch(_host_config);
	}

}
