#include "line.h"
#pragma warning(disable:4244)

#include "../stats.h"
#include <thread>
#include <mutex>

namespace SimplePlot::Line {
	template<typename X, typename Y>
	Line<X, Y>::Line(X* xData, Y* yData, int sizeData, int style, std::wstring name)
		: Plot(PLOT_TYPE::LINE, AXIS_TYPE::CART_2D, style, name), xData(xData), yData(yData), sizeData(sizeData) {
	}

	template<typename X, typename Y>
	Line<X, Y>::~Line() {

	}

	template<typename X, typename Y>
	void Line<X, Y>::getAxisLimits(float* axisLimits) const {
		axisLimits[0] = (float)SimplePlot::Stats::minValue<X>(xData, sizeData);
		axisLimits[1] = (float)SimplePlot::Stats::maxValue<X>(xData, sizeData);
		axisLimits[2] = (float)SimplePlot::Stats::minValue<Y>(yData, sizeData);
		axisLimits[3] = (float)SimplePlot::Stats::maxValue<Y>(yData, sizeData);
	}


	template<typename X, typename Y>
	void Line<X, Y>::draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const {
		// axisLimits: {minX, maxX, minY, maxY}
		// axisPoints: {origin, endX, endY, farCorner}
		SelectObject(hdc, style.forePen);

		LONG x = drawSpace[0].x + ((float)xData[0] - axisLimits[0]) / (axisLimits[1] - axisLimits[0]) * (drawSpace[1].x - drawSpace[0].x);
		LONG y = drawSpace[0].y + ((float)yData[0] - axisLimits[2]) / (axisLimits[3] - axisLimits[2]) * (drawSpace[2].y - drawSpace[0].y);
		MoveToEx(hdc, x, y, NULL);
		for (int i = 1; i < sizeData; i++) {
			LONG x = drawSpace[0].x + ((float)xData[i] - axisLimits[0]) / (axisLimits[1] - axisLimits[0]) * (drawSpace[1].x - drawSpace[0].x);
			LONG y = drawSpace[0].y + ((float)yData[i] - axisLimits[2]) / (axisLimits[3] - axisLimits[2]) * (drawSpace[2].y - drawSpace[0].y);
			switch (style.foreStyle) {
			case SP_SOLID:
				LineTo(hdc, x, y);
				break;
			case SP_DASH:
				if ((x / 10) % 2 == 0) { LineTo(hdc, x, y); }
				else { MoveToEx(hdc, x, y, NULL); }
				break;
			case SP_DOT:
				if ((x / 2) % 2 == 0) { LineTo(hdc, x, y); }
				else { MoveToEx(hdc, x, y, NULL); }
				break;
			case SP_DASHDOT:
				if ((x / 10) % 2 == 0) { LineTo(hdc, x, y); }
				else {
					if ((x / 10) == 4 || (x / 10) == 5) { LineTo(hdc, x, y); }
					else { MoveToEx(hdc, x, y, NULL); }
				}
				break;
			case SP_DASHDOTDOT:
				if ((x / 10) % 3 == 0) { LineTo(hdc, x, y); }
				else {
					if ((x / 10) % 3 == 1 && ((x / 10) == 6 || (x / 10) == 7)) { LineTo(hdc, x, y); }
					else if ((x / 10) % 3 == 2 && ((x / 10) == 2 || (x / 10) == 3)) { LineTo(hdc, x, y); }
					else { MoveToEx(hdc, x, y, NULL); }
				}
				break;
			default:
				break;
			}
		}
	}



	template<typename X, typename Y>
	void Line<X, Y>::isolateData() {
		X* newX = new X[sizeData];
		memcpy(newX, xData, sizeof(X) * sizeData);
		xData = newX;

		Y* newY = new Y[sizeData];
		memcpy(newY, yData, sizeof(Y) * sizeData);
		yData = newY;
	}

	template<typename X, typename Y>
	void Line<X, Y>::deleteData() {
		delete[] xData;
		delete[] yData;
	}


	template class Line<float, float>;
	template class Line<double, float>;
	template class Line<int, float>;
	template class Line<float, double>;
	template class Line<double, double>;
	template class Line<int, double>;
	template class Line<float, int>;
	template class Line<double, int>;
	template class Line<int, int>;
}



namespace SimplePlot {
	template<typename X, typename Y>
	PLOT_ID makeLine(X* x, Y* y, int sizeData, int style, std::wstring name) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Line::Line(x, y, sizeData, style, name);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::LINE);
		return id;
	}

	template PLOT_ID makeLine<float, float>(float* x, float* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<double, float>(double* x, float* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<int, float>(int* x, float* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<float, double>(float* x, double* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<double, double>(double* x, double* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<int, double>(int* x, double* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<float, int>(float* x, int* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<double, int>(double* x, int* y, int sizeData, int style, std::wstring name);
	template PLOT_ID makeLine<int, int>(int* x, int* y, int sizeData, int style, std::wstring name);
}