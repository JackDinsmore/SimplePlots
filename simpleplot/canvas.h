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
			Canvas(std::vector<PLOT_ID> plots_, SimplePlot::STYLE const* style);
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

			std::string title;
			std::string* axisTitles;

			HWND hwnd;
			CANVAS_ID id = NULL;

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
			float* axisLimits;
			POINT* drawSpace;
			SimplePlot::STYLE const* style;
			Axis* axes;
			bool killed = false;
		};
	}

	CANVAS_ID makeCanvas(std::vector<PLOT_ID> plots, SimplePlot::STYLE const* style = nullptr);
	void deleteCanvas(CANVAS_ID id);
	void addPlotToCanvas(CANVAS_ID canvasID, PLOT_ID plotID);
	void removePlotFromCanvas(CANVAS_ID canvasID, PLOT_ID plotID);
}
