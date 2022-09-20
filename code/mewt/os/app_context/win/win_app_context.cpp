
#include "mewt/os/app_context/win/win_app_context.h"

namespace mewt::os::win
{


	class AppContextWin : public app_context_t
	{
	public:
		using app_context_t::app_context_t;
		friend int APIENTRY ::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nShowCmd); // NOLINT
	};

}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nShowCmd) // NOLINT
{

	mewt::os::win::AppContextWin app_context({ ._instance = hInstance,
															 ._previous_instance = hPrevInstance,
															 ._command_line = lpCmdLine,
															 ._show_command = nShowCmd });
	return mewt::mewtMain(app_context);
}
