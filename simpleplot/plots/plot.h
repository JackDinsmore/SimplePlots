#pragma once
#include <string>
#include <windows.h>

#include "../standard.h"
#include "../colors.h"


namespace SimplePlot {
	namespace Plot {
		class Plot {
		public:
			Plot(PLOT_TYPE plotType, AXIS_TYPE axisType, int style, std::wstring name);
			Plot(const Plot&) = delete;
			Plot(Plot&&) = delete;
			Plot& operator=(Plot const&) = delete;
			Plot& operator=(Plot&&) = delete;

			virtual void isolateData() = 0;
			virtual void deleteData() = 0;
			virtual void getAxisLimits(float* axisLimits) const = 0;
			virtual void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const = 0;

			void drawLegend(HDC hdc, RECT legendRect);
			void getGeneralAxisLimits(float* axisLimits, bool set) const;

			PLOT_ID id = SP_NULL_PLOT;
			CANVAS_ID canvas = SP_NULL_CANVAS;
			AXIS_TYPE axisType;
			PLOT_TYPE plotType;
			int numAxes;
			float* setAxisLimits;
			bool* isSetAxisLimits;
			std::wstring name;

		protected:
			SimplePlot::Style::Style style;

		private:
			inline static PLOT_ID maxID = 0;
		};
	}

	void deletePlot(PLOT_ID plot);
	void registerPlot(PLOT_ID, Plot::Plot* plt, PLOT_TYPE plotType);

	AXIS_TYPE getPlotAxisType(PLOT_ID id);
	PLOT_TYPE getPlotType(PLOT_ID id);
	void getPlotAxisLimits(PLOT_ID id, float* axisLimits, bool set);
	void isolatePlotData(PLOT_ID id);
	void deletePlotData(PLOT_ID id);
	void drawPlot(PLOT_ID id, HDC hdc, float const* axisLimits, POINT const* drawSpace);
	void drawPlotLegend(PLOT_ID id, HDC hdc, RECT legendRect);
	void associatePlot(PLOT_ID plotID, CANVAS_ID canvasID);
	void disassociatePlot(PLOT_ID plotID);
	CANVAS_ID getPlotCanvas(PLOT_ID plotID);
	void setPlotAxisLimits(PLOT_ID id, int axisNum, float lowLimit, float highLimit);
	void setPlotLowerAxisLimit(PLOT_ID id, int axisNum, float lowLimit);
	void setPlotUpperAxisLimit(PLOT_ID id, int axisNum, float highLimit);
	std::wstring getPlotName(PLOT_ID id);
}