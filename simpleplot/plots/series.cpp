#include "Series.h"
#pragma warning(disable:4244)

#include "../stats.h"
#include <thread>
#include <mutex>

namespace SimplePlot::Series {
	template<typename Y>
	Series<Y>::Series(Y* data, int sizeData)
		: Plot(PLOT_TYPE::SERIES, AXIS_TYPE::CART_2D), data(data), sizeData(sizeData) {

	}

	template<typename Y>
	Series<Y>::~Series() {

	}

	template<typename Y>
	void Series<Y>::getAxisLimits(float* axisLimits) const {

	}

	template<typename Y>
	void Series<Y>::draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const {

	}

	template<typename Y>
	void Series<Y>::isolateData() {
		Y* newData = new Y[sizeData];
		memcpy(newData, data, sizeof(Y) * sizeData);
		data = newData;
	}

	template<typename Y>
	void Series<Y>::deleteData() {
		delete[] data;
	}


	template class Series<float>;
	template class Series<double>;
	template class Series<int>;
}



namespace SimplePlot {
	template<typename Y>
	PLOT_ID makeSeries(Y* data, int sizeData) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Series::Series(data, sizeData);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::SERIES);
		return id;
	}

	template PLOT_ID makeSeries<float>(float* data, int sizeData);
	template PLOT_ID makeSeries<double>(double* data, int sizeData);
	template PLOT_ID makeSeries<int>(int* data, int sizeData);
}