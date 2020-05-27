#pragma once
#include <string>
#include <windows.h>

#include "../standard.h"
#include "../colors.h"


namespace SimplePlot {
	namespace Plot {
		class Plot {
		public:
			Plot(PLOT_TYPE plotType, AXIS_TYPE axisType);
			~Plot();

			void loadStyle(STYLE const* style);

			virtual void isolateData() = 0;
			virtual void deleteData() = 0;
			virtual void getAxisLimits(float* axisLimits) const = 0;
			virtual void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const = 0;

			PLOT_ID id = NULL;
			CANVAS_ID canvas = NULL;
			AXIS_TYPE axisType;
			PLOT_TYPE plotType;

		protected:

			HBRUSH foreBrush;
			HBRUSH backBrush;

		private:
			inline static PLOT_ID maxID = 0;
		};
	}

	void deletePlot(PLOT_ID plot);
	void registerPlot(PLOT_ID, Plot::Plot* plt, PLOT_TYPE plotType);

	AXIS_TYPE getPlotAxisType(PLOT_ID id);
	PLOT_TYPE getPlotType(PLOT_ID id);
	void getPlotAxisLimits(PLOT_ID id, float* axisLimits);
	void isolatePlotData(PLOT_ID id);
	void deletePlotData(PLOT_ID id);
	void drawPlot(PLOT_ID id, HDC hdc, float const* axisLimits, POINT const* drawSpace);
	void associatePlot(PLOT_ID plotID, CANVAS_ID canvasID);
	void disassociatePlot(PLOT_ID plotID);
	CANVAS_ID getPlotCanvas(PLOT_ID plotID);
}