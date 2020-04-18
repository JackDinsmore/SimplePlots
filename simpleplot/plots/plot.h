#pragma once

#include "../standard.h"
#include "../wndProc.h"
#include "../colors.h"
#include "../axis.h"
#include "../stats.h"

namespace SimplePlot {
	namespace Plot {
		class Plot {
		public:
			inline Plot() {
				id = maxID;
				maxID++;
			}

			WPARAM launch();

			void inline kill() {
				std::lock_guard<std::mutex> guard(SimplePlot::hwndToBitmapMutex);
				DeleteObject(SimplePlot::hwndToBitmap[hwnd]);
			}

			Plot(std::string title) : Plot() {
				rename(title);
			}
			Plot(std::string title, int cx, int cy) : Plot() {
				rename(title);
				setSize(cx, cy);
			}

			inline void setPos(int x, int y) {
				POINT s = getSize();
				MoveWindow(hwnd, x, y, s.x, s.y, TRUE);
			}

			inline void setSize(int cx, int cy) {
				POINT p = getPos();
				MoveWindow(hwnd, p.x, p.y, cx, cy, TRUE);
			}

			inline void rename(std::string s) {
				SetWindowTextA(hwnd, s.c_str());
			}

			inline POINT getSize() {
				RECT rect;
				GetWindowRect(hwnd, &rect);
				return { rect.right - rect.left, rect.bottom - rect.top };
			}

			inline POINT getPos() {
				RECT rect;
				GetWindowRect(hwnd, &rect);
				return { rect.left, rect.top };
			}

			PLOT_ID id;

		protected:
			virtual void draw(HDC hdc) = 0;

		private:
			HWND hwnd;

			inline static PLOT_ID maxID = 0;

			void paint();
			void createBitmap();
		};
	}

	inline WPARAM launch(SimplePlot::Plot::Plot* plt) {
		return plt->launch();
	}
}