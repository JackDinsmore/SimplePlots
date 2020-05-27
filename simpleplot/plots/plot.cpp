#pragma comment(lib, "Shcore.lib")
#include "plot.h"
#include "../canvas.h"

#include <map>
#include <mutex>



namespace SimplePlot {
	namespace Maps {
		std::map<PLOT_ID, SimplePlot::Plot::Plot*> plotPointerMap;
		std::map<PLOT_ID, PLOT_TYPE> plotTypeMap;
		std::map<PLOT_ID, std::mutex> plotMutexMap;
		std::mutex mapMutex;
		class PlotGuard {
		public:
			PlotGuard(PLOT_ID id) : generalGuard(Maps::mapMutex), specificGuard(Maps::plotMutexMap[id]) {}

		private:
			std::lock_guard<std::mutex> generalGuard;
			std::lock_guard<std::mutex> specificGuard;
		};
	}

	namespace Plot {
		Plot::Plot(PLOT_TYPE plotType, AXIS_TYPE axisType)
			: plotType(plotType), axisType(axisType) {
			id = maxID;
			maxID++;
		}

		Plot::~Plot() {
			DeleteObject(foreBrush);
			DeleteObject(backBrush);
		}

		void Plot::loadStyle(STYLE const* style) {
			foreBrush = CreateSolidBrush(Color::getColor(style->foreBrushColor));
			backBrush = CreateSolidBrush(Color::getColor(style->backBrushColor));
		}
	}

	void deletePlot(PLOT_ID id) {
		std::lock_guard<std::mutex> g(Maps::mapMutex);
		Plot::Plot* plot = Maps::plotPointerMap[id];
		Maps::plotPointerMap.erase(id);
		Maps::plotMutexMap.erase(id);
		Maps::plotTypeMap.erase(id);
		if (plot->canvas != NULL) {
			removePlotFromCanvas(plot->canvas, id);
		}
		delete plot;
	}

	void registerPlot(PLOT_ID id, Plot::Plot* plt, PLOT_TYPE plotType) {
		std::lock_guard<std::mutex> g(Maps::mapMutex);
		Maps::plotPointerMap[id] = plt;
		Maps::plotTypeMap[id] = plotType;
		Maps::plotMutexMap[id];
	}

	AXIS_TYPE getPlotAxisType(PLOT_ID id) {
		Maps::PlotGuard guard(id);
		return Maps::plotPointerMap.at(id)->axisType;
	}

	PLOT_TYPE getPlotType(PLOT_ID id) {
		Maps::PlotGuard guard(id);
		return Maps::plotPointerMap.at(id)->plotType;
	}

	void getPlotAxisLimits(PLOT_ID id, float* axisLimits) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->getAxisLimits(axisLimits);
	}

	void isolatePlotData(PLOT_ID id) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->isolateData();
	}

	void deletePlotData(PLOT_ID id) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->deleteData();
	}

	void drawPlot(PLOT_ID id, HDC hdc, float const* axisLimits, POINT const* drawSpace) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->draw(hdc, axisLimits, drawSpace);
	}

	void associatePlot(PLOT_ID plotID, CANVAS_ID canvasID) {
		Maps::PlotGuard guard(plotID);
		Maps::plotPointerMap.at(plotID)->canvas = canvasID;
	}

	void disassociatePlot(PLOT_ID plotID) {
		Maps::PlotGuard guard(plotID);
		Maps::plotPointerMap.at(plotID)->canvas = NULL;
	}

	CANVAS_ID getPlotCanvas(PLOT_ID plotID) {
		Maps::PlotGuard guard(plotID);// Necessary?
		return Maps::plotPointerMap.at(plotID)->canvas;
	}
}