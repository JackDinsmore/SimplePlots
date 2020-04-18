#include "plot.h"


namespace SimplePlot {
	namespace Plot {
		WPARAM Plot::launch() {
			WNDCLASS windowClass = { 0 };
			windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hInstance = NULL;
			windowClass.lpfnWndProc = SimplePlot::wndProc::wndProc;
			windowClass.lpszClassName = L"Plot in Console";
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			if (!RegisterClass(&windowClass)) {
				MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
			}
			hwnd = CreateWindow(L"Plot in Console",
				NULL,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				SP_DEFAULT_WIDTH, SP_DEFAULT_HEIGHT,
				NULL,
				NULL,
				NULL,
				NULL);

			createBitmap();

			ShowWindow(hwnd, SW_SHOW);
			MSG messages;

			while (true) {
				paint();
				if (PeekMessage(&messages, hwnd, 0, 0, PM_REMOVE)) {
					TranslateMessage(&messages);
					DispatchMessage(&messages);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1000 / SP_FRAMERATE));

				std::lock_guard<std::mutex> guard(terminatePlotMutex);
				if (terminatePlot[hwnd]) {
					kill();
					break;
				}
			}
			DeleteObject(hwnd); //doing it just in case
			return messages.wParam;
		}

		void Plot::paint() {
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			HDC hdcScreen = GetDC(hwnd);
			HDC hdcBmp = CreateCompatibleDC(hdcScreen);
			SelectObject(hdcBmp, hwndToBitmap[hwnd]);

			draw(hdcBmp);

			DeleteDC(hdcBmp);
			ReleaseDC(NULL, hdcScreen);
		}

		void Plot::createBitmap() {
			RECT rc;
			GetClientRect(hwnd, &rc);
			HDC hdc = GetDC(hwnd);
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			std::lock_guard<std::mutex> guard2(terminatePlotMutex);
			hwndToBitmap[hwnd] = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
			terminatePlot[hwnd] = false;
		}
	}
}
