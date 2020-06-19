#pragma once
#include <windows.h>
#include <string>
#include <vector>

#include "standard.h"
#include "axis.h"

namespace SimplePlot {
	namespace Canvas {
		class Canvas {
		public:
			Canvas(std::vector<PLOT_ID> plots_, std::wstring name, STYLE const* style);
			~Canvas();

			void setPos(int x, int y);
			void setSize(int cx, int cy);
			void rename(std::string s);
			POINT getSize();
			POINT getPos();
			void setFramerate(int framerate_);

			void addPlot(PLOT_ID plotID);
			void removePlot(PLOT_ID plotID);
			void launch();
			bool isEmpty();
			void setGridLines(bool state);

			std::string title;

			HWND hwnd;
			CANVAS_ID id = SP_NULL_CANVAS;
			bool legend = false;
			bool enforceSquare = false;

		private:
			void initWindow();
			void paint();
			void draw(HDC hdc);
			void createBitmap();
			void setAxisType();
			void kill();

			inline static CANVAS_ID maxID;

			HBRUSH backBrush;
			std::vector<PLOT_ID> plots;
			int framerate = SP_DYNAMIC;
			AXIS_TYPE axisType;

			int numAxes = 0;
			int numCorners = 0;
			std::string* axisTitles;
			Axis* axes;
			float* axisLimits;
			POINT* drawSpace;
			std::wstring name;
			std::vector<std::wstring> plotNames;

			bool killed = false;
			SimplePlot::STYLE const* style;
		};
	}

	CANVAS_ID makeCanvas(std::vector<PLOT_ID> plots, std::wstring name = L"", SimplePlot::STYLE const* style = nullptr);
	void deleteCanvas(CANVAS_ID id);
	void addPlotToCanvas(CANVAS_ID canvasID, PLOT_ID plotID);
	void removePlotFromCanvas(CANVAS_ID canvasID, PLOT_ID plotID);
	void setCanvasGridLines(CANVAS_ID canvasID, bool state);
	void setCanvasFramerate(CANVAS_ID id, int framerate);
	void setCanvasLegend(CANVAS_ID id, bool legend);
	void setCanvasEnforceSquare(CANVAS_ID id, bool sq);
}
