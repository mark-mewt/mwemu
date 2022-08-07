
#include "mewt/gui/window/win/win_window.h"
#include "mewt/gui/window_manager/win/win_window_manager.h"

namespace mewt::gui {

	window_t window_i::create(window_manager_t& window_manager) {
		return std::make_unique<win::window_t>(static_cast<win::window_manager_t&>(*window_manager));
	}

	win::window_t::window_t(window_manager_t& window_manager) : _window_manager(window_manager) {

		_hwnd = ::CreateWindowExW(
			 WS_EX_APPWINDOW,
			 (LPCTSTR)(uintptr_t) _window_manager.window_class_atom(),
			 L"Mewt Window",
			 WS_CAPTION | WS_SIZEBOX | WS_VISIBLE,
			 CW_USEDEFAULT,
			 CW_USEDEFAULT,
			 400,
			 400,
			 nullptr,
			 nullptr,
			 _window_manager.app_context().init_data()._instance,
			 this);

	}

	void win::window_t::run() {
		MSG msg;
		BOOL bRet;
		while ((bRet = GetMessageW(&msg, NULL, 0, 0)) != 0) {
			if (bRet == -1) {
				throw std::exception("Windows Error");
			}
			else {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		// mwToDo: put msg.wParam into app context so it can be returned from WinMain.

	}

}

/*

	 ShowWindow(hwndMain, nCmdShow);
	 UpdateWindow(hwndMain);

	 // Start the message loop.

	 while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	 {
		  if (bRet == -1)
		  {
				// handle the error and possibly exit
		  }
		  else
		  {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		  }
	 }

	 // Return the exit code to the system.

	 return msg.wParam; 
	 
	 */