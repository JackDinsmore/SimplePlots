#pragma once
#include "standard.h"

#define SP_TICK_LENGTH 6


namespace SimplePlot {
	template<typename T>
	class Axis {
	public:
		Axis(std::string label = "", bool logarithmic = false, SimplePlot::Color::Color backColor = SimplePlot::Color::Color::WHITE, SimplePlot::Color::Color color = SimplePlot::Color::Color::BLACK) :
			label(label.begin(), label.end()), logarithmic(logarithmic), backColor(backColor), fixEnds(false), color(color) {
			makePen();
		}
		Axis(std::string label, T maxT, T minT, bool logarithmic = false, SimplePlot::Color::Color backColor = SimplePlot::Color::Color::WHITE, SimplePlot::Color::Color color = SimplePlot::Color::Color::BLACK) :
			label(label.begin(), label.end()), logarithmic(logarithmic), backColor(backColor), maxT(maxT), minT(minT), fixEnds(true) {
			makePen();
		}

		~Axis() {
			DeleteObject(thickPen);
			DeleteObject(thinPen);
			DeleteObject(backPen);
			DeleteObject(tickFont);
		}

		bool setEnds(T minT_, T maxT_) {
			if (!fixEnds) {
				minT = minT_;
				maxT = maxT_;
				setMajorMinor();
			}
			return fixEnds;
		}

		int getClearance() {
			/// TO DO: Implement
			return max(SP_BORDER_WIDTH, 50);
		}

		void draw(HDC hdc, POINT origin, POINT axisEnd, POINT gridEnd);

		bool grid = true;

	private:
		void setMajorMinor();

		void makePen() {
			thickPen = CreatePen(PS_SOLID, 2, Color::getColor(color));
			thinPen = CreatePen(PS_SOLID, 1, Color::getColor(color));
			COLORREF backCR = Color::getColor(backColor);
			COLORREF foreCR = Color::getColor(color);
			COLORREF penCR = RGB((GetRValue(backCR) + GetRValue(foreCR)) / 2, (GetGValue(backCR) + GetGValue(foreCR)) / 2, (GetBValue(backCR) + GetBValue(foreCR)) / 2);
			backPen = CreatePen(PS_SOLID, 1, penCR);

			tickFont = CreateFont(24, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
		}

		bool logarithmic = false;
		bool fixEnds;
		T maxT;
		T minT;
		T major;
		T minor;
		std::wstring label;
		SimplePlot::Color::Color color;
		SimplePlot::Color::Color backColor;
		HPEN thickPen, thinPen, backPen;

		HFONT labelFont;
		HFONT tickFont;
	};
}

#include "axis.ipp"