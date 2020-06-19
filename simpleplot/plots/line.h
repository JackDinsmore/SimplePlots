#pragma once
#include "plot.h"
#include "../axis.h"


namespace SimplePlot::Line {
	template<typename X, typename Y>
	class Line : public SimplePlot::Plot::Plot {
	public:
		Line(X* xData, Y* yData, int sizeData, STYLE const* style, std::wstring name);
		~Line();


	private:
		void getAxisLimits(float* axisLimits) const override;
		void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const override;
		void isolateData() override;
		void deleteData() override;

		X* xData;
		Y* yData;
		int sizeData;
	};
}

namespace SimplePlot {
	template<typename X, typename Y>
	extern PLOT_ID makeLine(X* x, Y* y, int sizeData, STYLE const* style = nullptr, std::wstring name = L"");
}