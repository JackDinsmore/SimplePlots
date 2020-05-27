#include "hist.h"
#pragma warning(disable:4244)

#include "../stats.h"
#include <thread>
#include <mutex>

namespace SimplePlot::Hist {
	template<typename Y>
	Hist<Y>::Hist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal) 
		: Plot(PLOT_TYPE::HISTOGRAM, AXIS_TYPE::CART_2D), data(data), sizeData(sizeData), numBins(numBins),
		normal(normal), maxBin(maxBin), minBin(minBin) {
		if (minBin >= maxBin) {
			throw std::invalid_argument("minBin must be < maxBin");
		}
	}

	template<typename Y>
	Hist<Y>::Hist(Y* data, int sizeData, Y* leftBins_, int numBins, bool normal)
		: Plot(PLOT_TYPE::HISTOGRAM, AXIS_TYPE::CART_2D), data(data), sizeData(sizeData), numBins(numBins), normal(normal) {
		if (numBins > 2) {
			throw std::invalid_argument("Num Bins must be >= 2");
		}
		leftBins = new Y[numBins];
		memcpy(leftBins, leftBins_, numBins * sizeof(Y));
	}

	template<typename Y>
	Hist<Y>::~Hist() {
		if (leftBins) delete[] leftBins;
	}


	template<typename Y>
	void Hist<Y>::getAxisLimits(float* axisLimits) const {
		// axisLimits: {minX, maxX, minY, maxY}
		axisLimits[0] = (float)SimplePlot::Stats::minValue(data, sizeData);
		axisLimits[1] = (float)SimplePlot::Stats::maxValue(data, sizeData);
		axisLimits[2] = 0;

		int* binCounts = new int[numBins];
		for (int i = 0; i < numBins; i++) {
			binCounts[i] = 0;
		}
		if (leftBins) {
			for (int i = 0; i < sizeData; i++) {
				binCounts[SimplePlot::Stats::binFindLeft<Y>(leftBins, numBins, data[i])]++;
			}
		}
		else {
			for (int i = 0; i < sizeData; i++) {
				int index = int((data[i] - minBin) / (maxBin - minBin) * numBins * (numBins / (numBins + 1.0f)));
				// The final term is to make the second bound inclusive

				if (index < 0 || index >= numBins) { continue; }
				binCounts[index]++;
			}
		}
		axisLimits[3] = SimplePlot::Stats::maxValue(binCounts, numBins);
	}

	template<typename Y>
	void Hist<Y>::draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const {
		// axisLimits: {minX, maxX, minY, maxY}
		// axisPoints: {origin, endX, endY, farCorner}

		int* binCounts = new int[numBins];
		for (int i = 0; i < numBins; i++) {
			binCounts[i] = 0;
		}
		if (leftBins) {
			for (int i = 0; i < sizeData; i++) {
				binCounts[SimplePlot::Stats::binFindLeft<Y>(leftBins, numBins, data[i])]++;
			}
		}
		else {
			for (int i = 0; i < sizeData; i++) {
				int index = int((data[i] - minBin) / (maxBin - minBin) * numBins * (numBins / (numBins + 1.0f)));
				// The final term is to make the second bound inclusive

				if (index < 0 || index >= numBins) { continue; }
				binCounts[index]++;
			}
		}

		const int maxCount = SimplePlot::Stats::maxValue(binCounts, numBins);
		const int minCount = 0;

		const POINT origin = drawSpace[0];
		const POINT endX = drawSpace[1];
		const POINT endY = drawSpace[2];
		const POINT farCorner = drawSpace[3];


		// Draw data
		MoveToEx(hdc, origin.x, origin.y, NULL);
		float pixelsPerBin = float(endX.x - origin.x) / numBins;
		for (int binNum = 0; binNum < numBins; binNum++) {
			LONG height = float(binCounts[binNum] - minCount) / (maxCount - minCount) * (origin.y - endY.y);
			RECT rect = { LONG(origin.x + pixelsPerBin * binNum), origin.y - height,
				LONG(origin.x + pixelsPerBin * (binNum + 1)), origin.y };
			FillRect(hdc, &rect, foreBrush);
			LineTo(hdc, rect.left, rect.top);
			LineTo(hdc, rect.right, rect.top);
		}
		LineTo(hdc, endX.x, endX.y);
	}

	template<typename Y>
	void Hist<Y>::isolateData() {
		Y* newData = new Y[sizeData];
		memcpy(newData, data, sizeof(Y) * sizeData);
		data = newData;
	}

	template<typename Y>
	void Hist<Y>::deleteData() {
		delete[] data;
	}


	template class Hist<float>;
	template class Hist<double>;
	template class Hist<int>;
}



namespace SimplePlot {
	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, numBins, minBin, maxBin, normal);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::HISTOGRAM);
		return id;
	}

	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, Y* leftBins, int numBins, bool normal) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, leftBins, numBins, normal);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::HISTOGRAM);
		return id;
	}

	template PLOT_ID makeHist<float>(float* data, int sizeData, int numBins, float minBin, float maxBin, bool normal);
	template PLOT_ID makeHist<double>(double* data, int sizeData, int numBins, double minBin, double maxBin, bool normal);
	template PLOT_ID makeHist<int>(int* data, int sizeData, int numBins, int minBin, int maxBin, bool normal);

	template PLOT_ID makeHist<float>(float* data, int sizeData, float* leftBins, int numBins, bool normal);
	template PLOT_ID makeHist<double>(double* data, int sizeData, double* leftBins, int numBins, bool normal);
	template PLOT_ID makeHist<int>(int* data, int sizeData, int* leftBins, int numBins,bool normal);
}