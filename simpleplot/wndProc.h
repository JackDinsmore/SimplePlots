#pragma once

#include "standard.h"


namespace SimplePlot {
	inline extern std::map<HWND, HBITMAP> hwndToBitmap = std::map<HWND, HBITMAP>();
	inline extern std::mutex hwndToBitmapMutex = std::mutex();

	inline extern std::map<HWND, bool> terminatePlot = std::map<HWND, bool>();
	inline extern std::mutex terminatePlotMutex = std::mutex();


	namespace wndProc {
		LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	}
}