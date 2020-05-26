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
			inline Plot(std::string title) : Plot() {
				rename(title);
			}
			inline Plot(std::string title, int cx, int cy) : Plot() {
				rename(title);
				setSize(cx, cy);
			}

			void launch();
			void kill();

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
				GetClientRect(hwnd, &rect);
				return { rect.right - rect.left, rect.bottom - rect.top };
			}

			inline POINT getPos() {
				RECT rect;
				GetWindowRect(hwnd, &rect);
				return { rect.left, rect.top };
			}

			inline float setFramerate(int framerate_) {
				if (framerate_ == SP_STATIC && framerate != SP_STATIC) {
					isolateData();
				}
				framerate = framerate_;
			}

			PLOT_ID id;

		protected:
			virtual void draw(HDC hdc) = 0;
			virtual void isolateData() = 0;
			virtual void deleteData() = 0;

		private:
			HWND hwnd;
			HBRUSH clearBrush;
			Color::Color clearColor = Color::Color::WHITE;

			inline static PLOT_ID maxID = 0;

			void paint();
			void createBitmap();

			int framerate = SP_DYNAMIC;
			bool killed = false;
		};
	}

	inline void launch(Plot::Plot* plt) {
		plt->launch();
	}

	void deletePlot(PLOT_ID plot);
	void registerPlot(PLOT_ID, Plot::Plot* plt, PLOT_TYPE plotType);
}