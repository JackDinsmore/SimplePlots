#pragma once
#pragma warning(disable:4244)
#include "plot.h"


namespace SimplePlot::Hist {
	template<typename Y>
	class Hist : public SimplePlot::Plot::Plot {
	public:
		Hist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false) : data(data), sizeData(sizeData), numBins(numBins), 
			vert("y"), horiz("x"), normal(normal), maxBin(maxBin), minBin(minBin) {
			if (minBin >= maxBin) {
				throw std::invalid_argument("minBin must be < maxBin");
			}
			initPlot();
		}
		Hist(Y* data, int sizeData, Y* leftBins_, int numBins, bool normal = false) : data(data), sizeData(sizeData), numBins(numBins),
			vert("y"), horiz("x"), normal(normal) {
			if (numBins > 2) {
				throw std::invalid_argument("Num Bins must be >= 2");
			}
			leftBins = new Y[numBins];
			memcpy(leftBins, leftBins_, numBins * sizeof(Y));
			initPlot();
		}

		~Hist() {
			if(leftBins) delete[] leftBins;
		}


	private:
		void initPlot() {
			fillBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		}

		void draw(HDC hdc) override {
			Y minT = SimplePlot::Stats::minValue(data, sizeData);
			Y maxT = SimplePlot::Stats::maxValue(data, sizeData);

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
			vert.setEnds(minCount, maxCount);
			horiz.setEnds(minT, maxT);
			const int clearanceVert = vert.getClearance();
			const int clearanceHoriz = horiz.getClearance();

			POINT size = getSize();

			const POINT origin = { clearanceVert, size.y - clearanceHoriz };
			const POINT endY = { clearanceVert, SP_BORDER_WIDTH };
			const POINT endX = { size.x - SP_BORDER_WIDTH, size.y - clearanceHoriz };
			const POINT farCorner = { size.x - SP_BORDER_WIDTH, SP_BORDER_WIDTH };

			vert.draw(hdc, origin, endY, endX);
			horiz.draw(hdc, origin, endX, endY);


			// Draw data
			MoveToEx(hdc, origin.x, origin.y, NULL);
			float pixelsPerBin = float(endX.x - origin.x) / numBins;
			for (int binNum = 0; binNum < numBins; binNum++) {
				LONG height = float(binCounts[binNum] - minCount) / (maxCount - minCount) * (origin.y - endY.y);
				RECT rect = {LONG(origin.x + pixelsPerBin * binNum), origin.y - height,
					LONG(origin.x + pixelsPerBin * (binNum + 1)), origin.y };
				FillRect(hdc, &rect, fillBrush);
				LineTo(hdc, rect.left, rect.top);
				LineTo(hdc, rect.right, rect.top);
			}
			LineTo(hdc, endX.x, endX.y);
		}

		void isolateData() override {
			Y* newData = new Y[sizeData];
			memcpy(newData, data, sizeof(Y) * sizeData);
			data = newData;
		}

		void deleteData() override {
			delete[] data;
		}

		Y* data;
		int sizeData;
		Y* leftBins = nullptr;
		Y minBin;
		Y maxBin;
		int numBins;
		Axis<int> vert;
		Axis<Y> horiz;
		bool normal;
		HBRUSH fillBrush;
	};
}

namespace SimplePlot {
	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, numBins, minBin, maxBin, normal);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::HISTOGRAM);
		std::thread(launch, plt).detach();
		return id;
	}

	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, Y* leftBins, int numBins, bool normal = false) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, leftBins, numBins, normal);
		PLOT_ID id = plt->id;
		registerPlot(id, plt, PLOT_TYPE::HISTOGRAM);
		std::thread(launch, plt).detach();
		return id;
	}
}