#include "line.h"
#pragma warning(disable:4244)

#include "../stats.h"
#include <thread>
#include <mutex>

namespace SimplePlot::Line {
	template<typename X, typename Y>
	Line<X, Y>::Line(X* x, Y* y, int sizeX, int sizeY)
		: Plot(PLOT_TYPE::LINE, AXIS_TYPE::CART_2D), x(x), y(y), sizeX(sizeX), sizeY(sizeY) {
	}

	template<typename X, typename Y>
	Line<X, Y>::~Line() {

	}

	template<typename X, typename Y>
	void Line<X, Y>::getAxisLimits(float* axisLimits) const {
		
	}


	template<typename X, typename Y>
	void Line<X, Y>::draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const {

	}

	template<typename X, typename Y>
	void Line<X, Y>::isolateData() {
		X* newX = new X[sizeX];
		memcpy(newX, x, sizeof(X) * sizeX);
		x = newX;

		Y* newY = new Y[sizeY];
		memcpy(newY, y, sizeof(Y) * sizeY);
		y = newY;
	}

	template<typename X, typename Y>
	void Line<X, Y>::deleteData() {
		delete[] x;
		delete[] y;
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
	PLOT_ID makeLine(X* x, Y* y, int sizeX, int sizeY) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Line::Line(x, y, sizeX, sizeY);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::LINE);
		return id;
	}

	template PLOT_ID makeLine<float, float>(float* x, float* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<double, float>(double* x, float* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<int, float>(int* x, float* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<float, double>(float* x, double* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<double, double>(double* x, double* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<int, double>(int* x, double* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<float, int>(float* x, int* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<double, int>(double* x, int* y, int sizeX, int sizeY);
	template PLOT_ID makeLine<int, int>(int* x, int* y, int sizeX, int sizeY);
}