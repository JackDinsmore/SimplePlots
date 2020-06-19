#include "wndProc.h"
#include <shobjidl.h>


namespace SimplePlot {
	namespace wndProc {
		void copyWindow(HWND hwnd);
		void saveWindow(HWND hwnd);

		LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
			RECT r;
			GetWindowRect(hwnd, &r);
			PAINTSTRUCT ps;
			HDC hdcMem;

			if (message == WM_DESTROY) {
				std::lock_guard<std::mutex> guard(terminateCanvasMutex);
				terminateCanvas[hwnd] = true;
				return 0;
			}

			if (message == WM_RBUTTONDOWN) {
				copyWindow(hwnd);
			}

			switch(message){
			case WM_KEYDOWN:
				if (wparam == VK_ESCAPE) {
					DestroyWindow(hwnd);
				}
				if (wparam == 'S' && GetKeyState(VK_CONTROL) < 0) {
					saveWindow(hwnd);
				}
				if (wparam == 'C' && GetKeyState(VK_CONTROL) < 0) {
					copyWindow(hwnd);
				}
				return 0;

			case WM_PAINT:
				try {
					// using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
					std::lock_guard<std::mutex> guardBit(hwndToBitmapMutex);
					BeginPaint(hwnd, &ps);

					hdcMem = CreateCompatibleDC(ps.hdc);
					SelectObject(hdcMem, hwndToBitmap.at(hwnd));
					BitBlt(ps.hdc,
						0, 0,
						r.right - r.left, r.bottom - r.top,
						hdcMem,
						0, 0,
						SRCCOPY);
					DeleteDC(hdcMem);
					EndPaint(hwnd, &ps);
				}
				catch (std::logic_error&) {
					int debug = 0;
				}

				break;

			case WM_ERASEBKGND:
				return (LRESULT)1; // Say we handled it.

			default:
				return DefWindowProc(hwnd, message, wparam, lparam);
			}
			return 0;
		}

		void copyWindow(HWND hwnd) {
			std::lock_guard<std::mutex> guardBit(hwndToBitmapMutex);
			if (OpenClipboard(NULL)) {
				EmptyClipboard();
				DIBSECTION ds;
				if (GetObject(hwndToBitmap[hwnd], sizeof(DIBSECTION), &ds)) {
					SetClipboardData(CF_BITMAP, hwndToBitmap[hwnd]);
				}
				CloseClipboard();
			}
		}

		void saveHBitmap(HBITMAP hBitmap, LPCTSTR filePath) {
			HDC hDC;
			int iBits;
			WORD wBitCount;
			DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
			BITMAP Bitmap0;
			BITMAPFILEHEADER bmfHdr;
			BITMAPINFOHEADER bi;
			LPBITMAPINFOHEADER lpbi;
			HANDLE fh, hDib, hPal, hOldPal2 = NULL;
			hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
			iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
			DeleteDC(hDC);
			if (iBits <= 1)
				wBitCount = 1;
			else if (iBits <= 4)
				wBitCount = 4;
			else if (iBits <= 8)
				wBitCount = 8;
			else
				wBitCount = 24;
			GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
			bi.biSize = sizeof(BITMAPINFOHEADER);
			bi.biWidth = Bitmap0.bmWidth;
			bi.biHeight = -Bitmap0.bmHeight;
			bi.biPlanes = 1;
			bi.biBitCount = wBitCount;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 0;
			bi.biYPelsPerMeter = 0;
			bi.biClrImportant = 0;
			bi.biClrUsed = 256;
			dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8 * Bitmap0.bmHeight;
			hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
			lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
			*lpbi = bi;

			hPal = GetStockObject(DEFAULT_PALETTE);
			if (hPal) {
				hDC = GetDC(NULL);
				hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
				RealizePalette(hDC);
			}


			GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
				(BITMAPINFO*)lpbi, DIB_RGB_COLORS);

			if (hOldPal2) {
				SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
				RealizePalette(hDC);
				ReleaseDC(NULL, hDC);
			}

			fh = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

			if (fh == INVALID_HANDLE_VALUE) {
				return;
			}

			bmfHdr.bfType = 0x4D42; // "BM"
			dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
			bmfHdr.bfSize = dwDIBSize;
			bmfHdr.bfReserved1 = 0;
			bmfHdr.bfReserved2 = 0;
			bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

			WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

			WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
			GlobalUnlock(hDib);
			GlobalFree(hDib);
			CloseHandle(fh);
		}

		void saveWindow(HWND hwnd) {
			hwndToBitmapMutex.lock();
			HBITMAP hbitmap = hwndToBitmap[hwnd];
			hwndToBitmapMutex.unlock();

			IFileOpenDialog* pFileSave;
			HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
				IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

			if (SUCCEEDED(hr)) {
				hr = pFileSave->Show(NULL);
				if (SUCCEEDED(hr)) {
					IShellItem* pItem;
					hr = pFileSave->GetResult(&pItem);
					if (SUCCEEDED(hr)) {
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						if (SUCCEEDED(hr)) {
							saveHBitmap(hbitmap, pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileSave->Release();
			}
		}
	}

}