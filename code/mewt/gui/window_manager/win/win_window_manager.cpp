
#include "mewt/gui/window_manager/win/win_window_manager.h"
#include "mewt/diag/log.h"

namespace mewt::gui {

	window_manager_t window_manager_i::create(const os::app_context_i& app_context) {
		return std::make_unique<win::window_manager_t>(static_cast<const os::win::app_context_t&>(app_context));
	}

	static LRESULT window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {

		switch (message) {
		case WM_GETMINMAXINFO:
		case WM_NCCREATE:
		case WM_NCCALCSIZE:
		case WM_CREATE:
		case WM_SHOWWINDOW:
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
		case WM_SIZE:
		case WM_MOVE:
		case WM_ACTIVATEAPP:
		case WM_NCACTIVATE:
		case WM_ACTIVATE:
		case WM_IME_SETCONTEXT:
		case WM_IME_NOTIFY:
		case WM_GETOBJECT:
		case WM_SETFOCUS:
		case WM_NCPAINT:
		case WM_ERASEBKGND:
		case WM_GETICON:
		case WM_SYNCPAINT:
		case WM_DWMNCRENDERINGCHANGED:
		case WM_DEVICECHANGE:
		case WM_KILLFOCUS:
		//case WM_PAINT:
		case WM_NCHITTEST:
		case WM_SETCURSOR:
		case WM_NCMOUSEMOVE:
		case WM_NCMOUSELEAVE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_CHAR:
		case WM_CONTEXTMENU:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_SYSCOMMAND:
		case WM_CANCELMODE:
		case WM_CAPTURECHANGED:
		case WM_ENTERSIZEMOVE:
		case WM_EXITSIZEMOVE:
		case WM_MOVING:
		case WM_SIZING:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_HELP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_NCDESTROY:
			// mwToDo: handle
			break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(window, &ps);
		}
			return 0;
		default:
			//logger().log("unhandled message: %d", message);
			break;
		}
		return ::DefWindowProcW(window, message, w_param, l_param);
	}

	win::window_manager_t::window_manager_t(const os::win::app_context_t& app_context) : _app_context(app_context) {

		WNDCLASSEXW windowClass = {
			.cbSize = sizeof(WNDCLASSEXW),
			.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW,
			.lpfnWndProc = window_procedure,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = app_context.init_data()._instance,
			.hIcon = nullptr,	// mwToDo: get icon
			.hCursor = LoadCursorW(app_context.init_data()._instance, IDC_HAND),
			.hbrBackground = nullptr, // mwToDo: need to handle WM_ERASEBKGND
			.lpszMenuName = nullptr,
			.lpszClassName = L"mewt_window_class",
			.hIconSm = nullptr,
		};
		_window_class_atom = ::RegisterClassExW(&windowClass);

	}

}
