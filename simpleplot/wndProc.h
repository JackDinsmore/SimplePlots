#pragma once

#include "standard.h"
#include <mutex>
#include <map>
#include <windows.h>


namespace SimplePlot {
	inline extern std::map<HWND, HBITMAP> hwndToBitmap = std::map<HWND, HBITMAP>();
	inline extern std::mutex hwndToBitmapMutex = std::mutex();

	inline extern std::map<HWND, bool> terminateCanvas = std::map<HWND, bool>();
	inline extern std::mutex terminateCanvasMutex = std::mutex();

	namespace wndProc {
		LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	}
}