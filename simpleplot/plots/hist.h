#pragma once
#include "plot.h"
#include "../axis.h"


namespace SimplePlot::Hist {
	template<typename Y>
	class Hist : public SimplePlot::Plot::Plot {
	public:
		Hist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false);
		Hist(Y* data, int sizeData, Y* leftBins_, int numBins, bool normal = false);

		~Hist();


	private:
		void getAxisLimits(float* axisLimits) const override;
		void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const override;
		void isolateData() override;
		void deleteData() override;

		Y* data;
		int sizeData;
		Y* leftBins = nullptr;
		Y minBin;
		Y maxBin;
		int numBins;
		bool normal;
	};
}

namespace SimplePlot {
	template<typename Y>
	extern PLOT_ID makeHist(Y* data, int sizeData, int numBins, Y minBin, Y maxBin, bool normal = false);

	template<typename Y>
	extern PLOT_ID makeHist(Y* data, int sizeData, Y* leftBins, int numBins, bool normal = false);
}