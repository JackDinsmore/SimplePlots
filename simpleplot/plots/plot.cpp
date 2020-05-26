#include "plot.h"
#pragma comment(lib, "Shcore.lib")
#include <shellscalingapi.h>


namespace SimplePlot {
	namespace Maps {
		std::map<PLOT_ID, SimplePlot::Plot::Plot*> plotPointerMap;
		std::map<PLOT_ID, PLOT_TYPE> plotTypeMap;
	}

	namespace Plot {
		void Plot::launch() {
			SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

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
				terminatePlotMutex.lock();
				bool killNow = terminatePlot.find(hwnd) == terminatePlot.end() || terminatePlot.at(hwnd);
				terminatePlotMutex.unlock();
				if (killNow) {
					kill();
					break;
				}

				paint();
				if (PeekMessage(&messages, hwnd, 0, 0, PM_REMOVE)) {
					TranslateMessage(&messages);
					DispatchMessage(&messages);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1000 / framerate));
			}
			DeleteObject(hwnd); // doing it just in case
			delete this;
		}

		void Plot::kill() {
			if (killed) { return; }
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			std::lock_guard<std::mutex> guard2(terminatePlotMutex);
			DeleteObject(hwndToBitmap[hwnd]);
			DeleteObject(clearBrush);
			terminatePlot[hwnd] = true;
			if (framerate == SP_STATIC) {
				deleteData();
			}
			killed = true;
		}

		void Plot::paint() {
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			HDC hdcScreen = GetDC(hwnd);
			HDC hdcBmp = CreateCompatibleDC(hdcScreen);


			RECT r;
			GetClientRect(hwnd, &r);

			SelectObject(hdcBmp, hwndToBitmap[hwnd]);

			HBRUSH oldBrush = (HBRUSH)SelectObject(hdcBmp, clearBrush);
			FillRect(hdcBmp, &r, clearBrush);
			//FillRect(hdcBmp, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
			SetBkMode(hdcBmp, TRANSPARENT);

			draw(hdcBmp);

			SelectObject(hdcBmp, oldBrush);

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

			clearBrush = CreateSolidBrush(Color::getColor(clearColor));
		}
	}

	void deletePlot(PLOT_ID id) {
		Maps::plotPointerMap[id]->kill();
	}

	void registerPlot(PLOT_ID id, Plot::Plot* plt, PLOT_TYPE plotType) {
		Maps::plotPointerMap[id] = plt;
		Maps::plotTypeMap[id] = plotType;
	}
}
