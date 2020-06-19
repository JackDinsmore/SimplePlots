#include "Series.h"
#pragma warning(disable:4244)

#include "../stats.h"
#include <thread>
#include <mutex>

namespace SimplePlot::Series {
	template<typename X, typename Y>
	Series<X, Y>::Series(X skip, Y* data, int sizeData, int style, std::wstring name)
		: Plot(PLOT_TYPE::SERIES, AXIS_TYPE::CART_2D, style, name), skip(skip), data(data), sizeData(sizeData) {
	}

	template<typename X, typename Y>
	Series<X, Y>::~Series() {

	}

	template<typename X, typename Y>
	void Series<X, Y>::getAxisLimits(float* axisLimits) const {
		axisLimits[0] = 0;
		axisLimits[1] = (float)(sizeData - 1) * skip;
		axisLimits[2] = (float)SimplePlot::Stats::minValue<Y>(data, sizeData);
		axisLimits[3] = (float)SimplePlot::Stats::maxValue<Y>(data, sizeData);
	}

	template<typename X, typename Y>
	void Series<X, Y>::draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const {
		// axisLimits: {minX, maxX, minY, maxY}
		// axisPoints: {origin, endX, endY, farCorner}
		SelectObject(hdc, style.forePen);

		LONG y = drawSpace[0].y + (data[0] - axisLimits[2]) / (axisLimits[3] - axisLimits[2]) * (drawSpace[2].y - drawSpace[0].y);
		MoveToEx(hdc, drawSpace[0].x, y, NULL);
		for (int i = 1; i < sizeData; i++) {
			LONG x = drawSpace[0].x + (i * skip) / (axisLimits[1] - axisLimits[0])  *(drawSpace[1].x - drawSpace[0].x);
			LONG y = drawSpace[0].y + ((float)data[i] - axisLimits[2]) / (axisLimits[3] - axisLimits[2]) * (drawSpace[2].y - drawSpace[0].y);
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
	void Series<X, Y>::isolateData() {
		Y* newData = new Y[sizeData];
		memcpy(newData, data, sizeof(Y) * sizeData);
		data = newData;
	}

	template<typename X, typename Y>
	void Series<X, Y>::deleteData() {
		delete[] data;
	}


	template class Series<float, float>;
	template class Series<float, double>;
	template class Series<float, int>;
	template class Series<double, float>;
	template class Series<double, double>;
	template class Series<double, int>;
	template class Series<int, float>;
	template class Series<int, double>;
	template class Series<int, int>;
}



namespace SimplePlot {
	template<typename X, typename Y>
	PLOT_ID makeSeries(X skip, Y* data, int sizeData, int style, std::wstring name) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Series::Series(skip, data, sizeData, style, name);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::SERIES);
		return id;
	}

	template PLOT_ID makeSeries<float, float>(float skip, float* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<float, double>(float skip, double* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<float, int>(float skip, int* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<double, float>(double skip, float* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<double, double>(double skip, double* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<double, int>(double skip, int* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<int, float>(int skip, float* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<int, double>(int skip, double* data, int sizeData, int style, std::wstring name);
	template PLOT_ID makeSeries<int, int>(int skip, int* data, int sizeData, int style, std::wstring name);
}