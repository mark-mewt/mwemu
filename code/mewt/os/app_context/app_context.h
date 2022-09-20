
#pragma once

#include "mewt/os/app_context/app_context.decl.h"

namespace mewt::os {

	struct app_context_i {

	protected:
		inline virtual ~app_context_i() {}
	};

}

namespace mewt {
	extern int mewtMain(const os::app_context_i& app_context);
}
