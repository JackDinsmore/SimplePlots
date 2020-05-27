#pragma once
#include "plot.h"
#include "../axis.h"


namespace SimplePlot::Line {
	template<typename X, typename Y>
	class Line : public SimplePlot::Plot::Plot {
	public:
		Line(X* x, Y* y, int sizeX, int sizeY);
		~Line();


	private:
		void getAxisLimits(float* axisLimits) const override;
		void draw(HDC hdc, float const* axisLimits, POINT const* drawSpace) const override;
		void isolateData() override;
		void deleteData() override;

		X* x;
		Y* y;
		int sizeX;
		int sizeY;
		bool normal;
	};
}

namespace SimplePlot {
	template<typename X, typename Y>
	extern PLOT_ID makeLine(X* x, Y* y, int sizeX, int sizeY);
}