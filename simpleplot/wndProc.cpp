#include "wndProc.h"

namespace SimplePlot {
	namespace wndProc {
		LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
			RECT r;
			GetWindowRect(hwnd, &r);
			PAINTSTRUCT ps;
			HDC hdcMem;

			switch (message) {
			case WM_CHAR: //this is just for a program exit besides window's borders/task bar
				if (wparam == VK_ESCAPE)
				{
					DestroyWindow(hwnd);
				}
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			case WM_PAINT:
				try {
					// using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
					std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
					BeginPaint(hwnd, &ps);

					hdcMem = CreateCompatibleDC(ps.hdc);
					SelectObject(hdcMem, hwndToBitmap[hwnd]);
					BitBlt(ps.hdc,
						0, 0,
						r.right - r.left, r.bottom - r.top,
						hdcMem,
						0, 0,
						SRCCOPY);
					DeleteDC(hdcMem);
					EndPaint(hwnd, &ps);
					SelectObject(0, NULL);
				}
				catch (std::logic_error&) {
					int debug = 0;
				}

				InvalidateRect(hwnd, &r, FALSE);
				break;

			case WM_ERASEBKGND:
				return (LRESULT)1; // Say we handled it.

			case WM_QUIT:
				try {
					// using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
					std::lock_guard<std::mutex> guard(terminatePlotMutex);
					terminatePlot[hwnd] = true;
				}
				catch (std::logic_error&) {
					int debug = 0;
				}
				break;

			default:
				return DefWindowProc(hwnd, message, wparam, lparam);
			}
			return 0;
		}
	}

}