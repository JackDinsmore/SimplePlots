#pragma once
#include "plot.h"


namespace SimplePlot::Hist {
	template<typename Y>
	class Hist : public SimplePlot::Plot::Plot {
	public:
		Hist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false) : data(data), sizeData(sizeData), numBins(numBins), 
			vert("y"), horiz("x"), normal(normal) {
			if (minBin >= maxBin) {
				throw std::invalid_argument("minBin must be < maxBin");
			}
			leftBins = new Y[numBins];
			for (int i = 0; i < numBins; i++) {
				leftBins[i] = minBin + (maxBin - minBin) / numBins;
			}
		}
		Hist(Y* data, int sizeData, Y* leftBins, int numBins, bool normal = false) : data(data), sizeData(sizeData), leftBins(leftBins), numBins(numBins),
			vert("y"), horiz("x"), normal(normal) {
			if (numBins > 2) {
				throw std::invalid_argument("Num Bins must be >= 2");
			}
		}
		~Hist() {
			delete[] leftBins;
		}

		void draw(HDC hdc) override {
			Y minT = SimplePlot::Stats::minValue(data, sizeData);
			Y maxT = SimplePlot::Stats::maxValue(data, sizeData);
			/// Bin data

			//vert.setEnds(minT, maxT);
			horiz.setEnds(minT, maxT);
			int clearanceVert = vert.getClearance();
			int clearanceHoriz = horiz.getClearance();

			POINT size = getSize();

			POINT origin = { clearanceVert, size.y - clearanceHoriz };
			POINT endY = { clearanceVert, SP_BUFFER_TOP };
			POINT endX = { size.x - SP_BUFFER_RIGHT, size.y - clearanceHoriz };
			POINT farCorner = { size.x - SP_BUFFER_RIGHT, SP_BUFFER_TOP };

			vert.draw(hdc, origin, endY, endX);
			horiz.draw(hdc, origin, endX, endY);

			/// Draw data
		}

	private:
		Y* data;
		int sizeData;
		Y* leftBins;
		int numBins;
		Axis<int> vert;
		Axis<Y> horiz;
		bool normal;
	};
}

namespace SimplePlot {
	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, numBins, minBin, maxBin, normal);
		PLOT_ID id = plt->id;
		std::thread(launch, plt).detach();
		return id;
	}

	template<typename Y>
	PLOT_ID makeHist(Y* data, int sizeData, Y* leftBins, int numBins, bool normal = false) {
		SimplePlot::Plot::Plot* plt = new SimplePlot::Hist::Hist(data, sizeData, leftBins, numBins, normal);
		PLOT_ID id = plt->id;
		std::thread(launch, plt).detach();
		return id;
	}
}