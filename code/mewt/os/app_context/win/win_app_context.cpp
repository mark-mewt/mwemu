
#include "mewt/os/app_context/win/win_app_context.h"

int APIENTRY WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PSTR cmdline, _In_ int cmdshow) {

	mewt::os::win::app_context_t app_context({ ._instance = hInst,
															 ._previous_instance = hInstPrev,
															 ._command_line = cmdline,
															 ._show_command = cmdshow });
	return mewt::mewt_main(app_context);
}
