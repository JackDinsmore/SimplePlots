#pragma once
#include "standard.h"

namespace SimplePlot {
	template<typename T>
	class Axis {
	public:
		Axis(std::string label = "", bool logarithmic = false, SimplePlot::Color::Color color = SimplePlot::Color::Color::BLACK) : 
			label(label), logarithmic(logarithmic), fixEnds(false), color(color) {}
		Axis(std::string label, T maxT, T minT, bool logarithmic = false, SimplePlot::Color::Color color = SimplePlot::Color::Color::BLACK) :
			label(label), logarithmic(logarithmic), maxT(maxT), minT(minT), fixEnds(true) {}

		bool setEnds(T minT_, T maxT_) {
			if (!fixEnds) {
				minT = minT_;
				maxT = maxT_;
				setMajorMinor();
			}
			return fixEnds;
		}

		int getClearance();

		void draw(HDC hdc, POINT origin, POINT axisEnd, POINT gridEnd);

	private:
		void setMajorMinor();

		bool logarithmic = false;
		bool fixEnds;
		T maxT;
		T minT;
		T major;
		T minor;
		std::string label;
		SimplePlot::Color::Color color;
	};
}