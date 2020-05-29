#include "canvas.h"
#pragma comment(lib, "Shcore.lib")
#pragma warning(disable:4267)

#include <shellscalingapi.h>
#include <thread>
#include <algorithm>

#include "wndProc.h"
#include "plots/plot.h"


namespace SimplePlot {
	namespace Maps {
		std::map<CANVAS_ID, HWND> canvasHWNDMap;
		std::map<CANVAS_ID, std::mutex> canvasMutexMap;
		std::map<CANVAS_ID, SimplePlot::Canvas::Canvas*> canvasPointerMap;
		std::mutex canvasMapMutex;

		class CanvasGuard {
		public:
			CanvasGuard(CANVAS_ID id) : generalGuard(Maps::canvasMapMutex), specificGuard(Maps::canvasMutexMap[id]) {}

		private:
			std::lock_guard<std::mutex> generalGuard;
			std::lock_guard<std::mutex> specificGuard;
		};
	}

	namespace Canvas {
		Canvas::Canvas(std::vector<PLOT_ID> plots_, std::wstring name, SimplePlot::STYLE const* style) : name(name), style(style) {
			id = maxID;
			maxID++;

			if (plots_.size() == 0) { return; }
			axisType = getPlotAxisType(plots_[0]);
			if (plots_.size() > 1 && axisType == AXIS_TYPE::SPECIAL) {
				throw std::logic_error("Plots with axis type \"special\" cannot be in the same canvas as other plots.");
			}
			for (PLOT_ID id : plots_) {
				if (axisType != getPlotAxisType(id)) {
					throw std::logic_error("All plots within a canvas must have the same axis type.");
				}
				addPlot(id);
			}

			setAxisType();

			if (!style) {
				style = &Style::grayscale;
			}
			backBrush = CreateSolidBrush(Color::getColor(style->backBrushColor));
		}

		Canvas::~Canvas() {
			delete[] axisTitles;
			delete[] axisLimits;
			delete[] drawSpace;
			delete[] axes;
			DeleteObject(backBrush);
		}

		void Canvas::initWindow() {
			SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

			wchar_t name[256];
			wsprintfW(name, L"Plot %d", id);

			WNDCLASS windowClass = { 0 };
			windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hInstance = NULL;
			windowClass.lpfnWndProc = SimplePlot::wndProc::wndProc;
			windowClass.lpszClassName = name;
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			if (!RegisterClass(&windowClass)) {
				DWORD err = GetLastError();
				MessageBox(NULL, (L"Could not register class. Error #" + std::to_wstring(err)).c_str(), L"Error", MB_OK);
			}
			hwnd = CreateWindow(name,
				NULL,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				SP_DEFAULT_WIDTH, SP_DEFAULT_HEIGHT,
				NULL,
				NULL,
				NULL,
				NULL);
			createBitmap();
			
			std::lock_guard<std::mutex> generalGuard(Maps::canvasMapMutex);
			Maps::canvasHWNDMap[id] = hwnd;

			ShowWindow(hwnd, SW_SHOW);
		}

		void Canvas::launch() {
			initWindow();

			MSG messages;
			while (true) {
				terminateCanvasMutex.lock();
				bool killNow = terminateCanvas.find(hwnd) == terminateCanvas.end() || terminateCanvas.at(hwnd);
				terminateCanvasMutex.unlock();
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

		void Canvas::paint() {
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			HDC hdcScreen = GetDC(hwnd);
			HDC hdcBmp = CreateCompatibleDC(hdcScreen);


			RECT r;
			GetClientRect(hwnd, &r);

			SelectObject(hdcBmp, hwndToBitmap[hwnd]);

			HBRUSH oldBrush = (HBRUSH)SelectObject(hdcBmp, backBrush);
			FillRect(hdcBmp, &r, backBrush);
			SetBkMode(hdcBmp, TRANSPARENT);

			draw(hdcBmp);

			SelectObject(hdcBmp, oldBrush);

			DeleteDC(hdcBmp);
			ReleaseDC(NULL, hdcScreen);
		}

		void Canvas::createBitmap() {
			RECT rc;
			GetClientRect(hwnd, &rc);
			HDC hdc = GetDC(hwnd);
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			std::lock_guard<std::mutex> guard2(terminateCanvasMutex);
			hwndToBitmap[hwnd] = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
			terminateCanvas[hwnd] = false;
		}

		void Canvas::setPos(int x, int y) {
			POINT s = getSize();
			MoveWindow(hwnd, x, y, s.x, s.y, TRUE);
		}

		void Canvas::setSize(int cx, int cy) {
			POINT p = getPos();
			MoveWindow(hwnd, p.x, p.y, cx, cy, TRUE);
		}

		void Canvas::rename(std::string s) {
			SetWindowTextA(hwnd, s.c_str());
		}

		POINT Canvas::getSize() {
			RECT rect;
			GetClientRect(hwnd, &rect);
			return { rect.right - rect.left, rect.bottom - rect.top };
		}

		POINT Canvas::getPos() {
			RECT rect;
			GetWindowRect(hwnd, &rect);
			return { rect.left, rect.top };
		}

		void Canvas::setFramerate(int framerate_) {
			if (framerate_ == SP_STATIC && framerate != SP_STATIC) {
				for (PLOT_ID id: plots) {
					isolatePlotData(id);
				}
			}
			if (framerate_ != SP_STATIC && framerate == SP_STATIC) {
				for (PLOT_ID id : plots) {
					deletePlotData(id);
				}
			}
			framerate = framerate_;
		}

		void Canvas::addPlot(PLOT_ID plotID) {
			CANVAS_ID originalCanvas = getPlotCanvas(plotID);
			if (originalCanvas != SP_NULL_CANVAS) {
				removePlotFromCanvas(originalCanvas, plotID);
			}
			associatePlot(plotID, id);

			if (plots.size() == 0) {
				plots.push_back(plotID);
				axisType = getPlotAxisType(plotID);
				setAxisType();
				return;
			}

			// Binary insert into a list which is sorted from greatest to least.
			int thisOrder = (int)getPlotType(plotID);
			auto begin = plots.begin();
			auto end = plots.end()-1;
			int beginOrder = (int)getPlotType(*begin);
			int endOrder = (int)getPlotType(*end);
			while (beginOrder != endOrder) {
				auto middle = (begin + std::distance(begin, end) / 2);
				int middleOrder = (int)getPlotType(*middle);
				if (middleOrder > thisOrder) {
					begin = middle;
					beginOrder = middleOrder;
				}
				else {
					end = middle;
					endOrder = middleOrder;
				}
			}
			if (beginOrder != thisOrder) {
				if (beginOrder < thisOrder) {
					plots.insert(begin, plotID);
				}
				else {
					plots.insert(end+1, plotID);
				}
			}
			else {
				plots.insert(begin, plotID);
			}
		}

		void Canvas::removePlot(PLOT_ID plotID) {
			auto it = std::find(plots.begin(), plots.end(), plotID);
			if (it == plots.end()) {
				// Plot is not in the canvas
				return;
			}
			disassociatePlot(plotID);
			plots.erase(it);
		}

		void Canvas::setAxisType() {
			switch (axisType) {
			case AXIS_TYPE::CART_2D:
				numAxes = 2;
				numCorners = 4;
				break;
			default:
				throw std::logic_error("Axis types other than cart 2d are unimplemented");
			}
			numAxes = Axes::getNumAxes(axisType);
			numCorners = Axes::getNumAxisCorners(axisType);
			axes = new Axis[numAxes];
			axisTitles = new std::string[numAxes];
			axisLimits = new float[numAxes * 2];
			drawSpace = new POINT[numCorners];
		}

		void Canvas::draw(HDC hdc) {
			for (int i = 0; i < plots.size(); i++) {
				getPlotAxisLimits(plots[i], axisLimits, i==0);
			}

			axes[0].setEnds(axisLimits[0], axisLimits[1]);
			axes[1].setEnds(axisLimits[2], axisLimits[3]);

			axes[0].drawGrid(hdc, drawSpace[0], drawSpace[1], drawSpace[2]);
			axes[1].drawGrid(hdc, drawSpace[0], drawSpace[2], drawSpace[1]);

			POINT size = getSize();
			int clearanceHoriz = axes[0].getClearance();
			int clearanceVert = axes[1].getClearance();
			drawSpace[0] = { clearanceVert, size.y - clearanceHoriz };
			drawSpace[1] = { size.x - SP_BORDER_WIDTH, size.y - clearanceHoriz };
			drawSpace[2] = { clearanceVert, SP_BORDER_WIDTH };
			drawSpace[3] = { size.x - SP_BORDER_WIDTH, SP_BORDER_WIDTH };

			for (PLOT_ID id : plots) {
				drawPlot(id, hdc, axisLimits, drawSpace);
			}
			axes[0].drawAxis(hdc, drawSpace[0], drawSpace[1], drawSpace[2]);
			axes[1].drawAxis(hdc, drawSpace[0], drawSpace[2], drawSpace[1]);

			RECT nameRect = { 0, 0, size.x, 80 };
			DrawText(hdc, name.c_str(), name.size(), &nameRect, DT_CENTER);
		}

		void Canvas::kill() {
			if (killed) { return; }
			std::lock_guard<std::mutex> guard(hwndToBitmapMutex);
			std::lock_guard<std::mutex> guard2(terminateCanvasMutex);
			DeleteObject(hwndToBitmap[hwnd]);
			terminateCanvas[hwnd] = true;
			if (framerate == SP_STATIC) {
				for (PLOT_ID id : plots) {
					deletePlotData(id);
				}
			}
			killed = true;
		}

		bool Canvas::isEmpty() {
			return plots.size() == 0;
		}

		void Canvas::setGridLines(bool state) {
			for (int i = 0; i < numAxes; i++) {
				axes[i].grid = state;
			}
		}
	}


	CANVAS_ID makeCanvas(std::vector<PLOT_ID> plots, std::wstring name, SimplePlot::STYLE const* style) {
		// Spawn the update function in a new thread.
		if (!style) {
			style = &Style::grayscale;
		}
		Canvas::Canvas* canvas = new Canvas::Canvas(plots, name, style);
		std::thread(&Canvas::Canvas::launch, canvas).detach();
		CANVAS_ID id = canvas->id;
		std::lock_guard<std::mutex> generalGuard(Maps::canvasMapMutex);
		Maps::canvasMutexMap[id];
		Maps::canvasPointerMap[id] = canvas;
		return id;
	}

	void deleteCanvas(CANVAS_ID id) {
		std::lock_guard<std::mutex> g(terminateCanvasMutex);
		terminateCanvas.at(Maps::canvasHWNDMap.at(id)) = true;
	}

	void addPlotToCanvas(CANVAS_ID canvasID, PLOT_ID plotID) {
		Maps::CanvasGuard guard(canvasID);
		Maps::canvasPointerMap.at(canvasID)->addPlot(plotID);
	}

	void removePlotFromCanvas(CANVAS_ID canvasID, PLOT_ID plotID) {
		Maps::CanvasGuard guard(canvasID);
		Maps::canvasPointerMap.at(canvasID)->removePlot(plotID);
	}

	void setCanvasGridLines(CANVAS_ID canvasID, bool state) {
		Maps::CanvasGuard guard(canvasID);
		Maps::canvasPointerMap.at(canvasID)->setGridLines(state);
	}

	void setCanvasFramerate(CANVAS_ID id, int framerate) {
		Maps::CanvasGuard guard(id);
		Maps::canvasPointerMap.at(id)->setFramerate(framerate);
	}
}