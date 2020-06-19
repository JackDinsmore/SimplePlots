#pragma once
#include "plot.h"
#include "../axis.h"


namespace SimplePlot::Series {
	template<typename X, typename Y>
	class Series : public SimplePlot::Plot::Plot {
	public:
		Series(X skip, Y* data, int sizeData, STYLE const* style, std::wstring name);
		~Series();


	private:
		void getAxisLimits(float* axisLimits) const override;
		void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const override;
		void isolateData() override;
		void deleteData() override;

		X skip;
		Y* data;
		int sizeData;
	};
}

namespace SimplePlot {
	template<typename X, typename Y>
	extern PLOT_ID makeSeries(X skip, Y* data, int sizeData, STYLE const* style = nullptr, std::wstring name = L"");
}