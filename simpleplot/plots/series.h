#pragma once
#include "plot.h"
#include "../axis.h"


namespace SimplePlot::Series {
	template<typename Y>
	class Series : public SimplePlot::Plot::Plot {
	public:
		Series(Y* data, int sizeData);
		~Series();


	private:
		void getAxisLimits(float* axisLimits) const override;
		void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const override;
		void isolateData() override;
		void deleteData() override;

		Y* data;
		int sizeData;
	};
}

namespace SimplePlot {
	template<typename Y>
	extern PLOT_ID makeSeries(Y* data, int sizeData);
}