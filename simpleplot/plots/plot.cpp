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
		Plot::Plot(PLOT_TYPE plotType, AXIS_TYPE axisType, int style, std::wstring name)
			: plotType(plotType), axisType(axisType), style(style), name(name) {
			id = maxID;
			maxID++;

			numAxes = Axes::getNumAxes(axisType);
			setAxisLimits = new float[numAxes * 2];
			isSetAxisLimits = new bool[numAxes * 2];
			for (int i = 0; i < numAxes * 2; i++) {
				isSetAxisLimits[i] = 0;
			}
		}

		void Plot::getGeneralAxisLimits(float* axisLimits, bool set) const {
			float* tempAxisLimits = new float[numAxes * 2];
			getAxisLimits(tempAxisLimits);

			for (int i = 0; i < numAxes * 2; i++) {
				if (isSetAxisLimits[i] != 0) {
					tempAxisLimits[i] = setAxisLimits[i];
				}
				if (set) {
					axisLimits[i] = tempAxisLimits[i];
				}
				else {
					if (i % 2 == 0) {
						axisLimits[i] = min(tempAxisLimits[i], axisLimits[i]);
					}
					else {
						axisLimits[i] = max(tempAxisLimits[i], axisLimits[i]);
					}
				}
			}
			delete[] tempAxisLimits;
		}

		void Plot::drawLegend(HDC hdc, RECT legendRect) {
			SelectObject(hdc, style.forePen);
			MoveToEx(hdc, legendRect.left, legendRect.top + 15, NULL);
			LineTo(hdc, legendRect.left + 15, legendRect.top + 15);
			legendRect.left += 20;
			DrawText(hdc, name.c_str(), (int)name.size(), &legendRect, NULL);
		}
	}

	void deletePlot(PLOT_ID id) {
		Maps::mapMutex.lock();/// Possibly not necessary.
		Plot::Plot* plot = Maps::plotPointerMap[id];
		Maps::mapMutex.unlock();

		if (plot->canvas != SP_NULL_CANVAS) {
			removePlotFromCanvas(plot->canvas, id);
		}

		std::lock_guard<std::mutex> guard(Maps::mapMutex);
		Maps::plotPointerMap.erase(id);
		Maps::plotMutexMap.erase(id);
		Maps::plotTypeMap.erase(id);

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

	void getPlotAxisLimits(PLOT_ID id, float* axisLimits, bool set) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->getGeneralAxisLimits(axisLimits, set);
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

	void drawPlotLegend(PLOT_ID id, HDC hdc, RECT legendRect) {
		Maps::PlotGuard guard(id);
		Maps::plotPointerMap.at(id)->drawLegend(hdc, legendRect);
	}

	void associatePlot(PLOT_ID plotID, CANVAS_ID canvasID) {
		Maps::PlotGuard guard(plotID);
		Maps::plotPointerMap.at(plotID)->canvas = canvasID;
	}

	void disassociatePlot(PLOT_ID plotID) {
		Maps::PlotGuard guard(plotID);
		Maps::plotPointerMap.at(plotID)->canvas = SP_NULL_CANVAS;
	}

	CANVAS_ID getPlotCanvas(PLOT_ID plotID) {
		Maps::PlotGuard guard(plotID);// Necessary?
		return Maps::plotPointerMap.at(plotID)->canvas;
	}

	void setPlotAxisLimits(PLOT_ID id, int axisNum, float lowLimit, float highLimit) {
		Maps::PlotGuard guard(id);// Necessary?
		Plot::Plot* ptr = Maps::plotPointerMap.at(id);
		if (ptr->numAxes <= axisNum) {
			return;
		}
		ptr->setAxisLimits[axisNum * 2] = lowLimit;
		ptr->isSetAxisLimits[axisNum * 2] = true;
		ptr->setAxisLimits[axisNum * 2 + 1] = highLimit;
		ptr->isSetAxisLimits[axisNum * 2 + 1] = true;
	}
	void setPlotLowerAxisLimit(PLOT_ID id, int axisNum, float lowLimit) {
		Maps::PlotGuard guard(id);// Necessary?
		Plot::Plot* ptr = Maps::plotPointerMap.at(id);
		if (ptr->numAxes <= axisNum) {
			return;
		}
		ptr->setAxisLimits[axisNum * 2] = lowLimit;
		ptr->isSetAxisLimits[axisNum * 2] = true;
	}
	void setPlotUpperAxisLimit(PLOT_ID id, int axisNum, float highLimit) {
		Maps::PlotGuard guard(id);// Necessary?
		Plot::Plot* ptr = Maps::plotPointerMap.at(id);
		if (ptr->numAxes <= axisNum) {
			return;
		}
		ptr->setAxisLimits[axisNum * 2 + 1] = highLimit;
		ptr->isSetAxisLimits[axisNum * 2 + 1] = true;
	}
	std::wstring getPlotName(PLOT_ID id) {
		Maps::PlotGuard guard(id);
		return Maps::plotPointerMap.at(id)->name;
	}
}