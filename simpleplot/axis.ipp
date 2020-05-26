#pragma once
#pragma warning(disable:4267)
#include "standard.h"

namespace SimplePlot {
	template<typename T>
	void Axis<T>::draw(HDC hdc, POINT origin, POINT axisEnd, POINT gridEnd) {
		HPEN oldPen = (HPEN)SelectObject(hdc, thickPen);
		MoveToEx(hdc, origin.x, origin.y, NULL);
		LineTo(hdc, axisEnd.x, axisEnd.y);

		SelectObject(hdc, thinPen);

		POINT gridAxis = { gridEnd.x - origin.x, gridEnd.y - origin.y };
		float gridLength = sqrt(gridAxis.x * gridAxis.x + gridAxis.y * gridAxis.y);
		POINT tick = { gridAxis.x / gridLength * SP_TICK_LENGTH, gridAxis.y / gridLength * SP_TICK_LENGTH };

		SelectObject(hdc, tickFont);

		if (!logarithmic) {
			for (int i = minT / minor; i * minor <= maxT; i++) {
				float frac = float(i * minor - minT) / (maxT - minT);
				POINT pos = { origin.x + frac * (axisEnd.x - origin.x), origin.y + frac * (axisEnd.y - origin.y) };
				if (i != 0) {
					MoveToEx(hdc, pos.x - tick.x, pos.y - tick.y, NULL);
					LineTo(hdc, pos.x + tick.x, pos.y + tick.y);

					// Grid lines
					if (grid) {
						SelectObject(hdc, backPen);
						LineTo(hdc, pos.x + gridAxis.x, pos.y + gridAxis.y);
						SelectObject(hdc, thinPen);
					}
				}

				// Numbers
				RECT widthRect = { 0, 0, 0, 0};
				std::wstring number = typename SimplePlot::Stats::template round<T>(i * minor, (int)log10(minor));
				DrawText(hdc, number.c_str(), number.size(), &widthRect, DT_CALCRECT);
				RECT textRect = { pos.x - LONG(widthRect.right / 2), pos.y -LONG(widthRect.bottom / 2),
					pos.x + LONG(widthRect.right / 2), pos.y + LONG(widthRect.bottom / 2) };
				DrawText(hdc, number.c_str(), number.size(), &textRect, DT_LEFT);
			}
		}
		else {
			throw std::logic_error("Not implemented");
		}

		// Box
		SelectObject(hdc, thickPen);
		MoveToEx(hdc, axisEnd.x, axisEnd.y, NULL);
		LineTo(hdc, axisEnd.x + gridAxis.x, axisEnd.y + gridAxis.y);


		// Draw axis labels
		float angle = atan2(axisEnd.y - origin.y, axisEnd.x - origin.x);
		int escapement = -angle * 180 / 3.14159265f * 10;

		labelFont = CreateFont(28, 0, escapement, escapement, 600, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
		HFONT oldFont = (HFONT)SelectObject(hdc, labelFont);
		RECT labelRect;
		POINT dropVector = { gridEnd.x - origin.x, gridEnd.y - origin.y };
		float dropScale = -50 / sqrt(dropVector.x * dropVector.x  + dropVector.y * dropVector.y);
		dropVector = { (LONG)(dropVector.x* dropScale), (LONG)(dropVector.y * dropScale) };
		POINT dropStart = { origin.x + dropVector.x, origin.y + dropVector.y };
		POINT dropEnd = { axisEnd.x + dropVector.x, axisEnd.y + dropVector.y };
		labelRect.top = min(min(min(origin.y, axisEnd.y), dropStart.y), dropEnd.y);
		labelRect.bottom = max(max(max(origin.y, axisEnd.y), dropStart.y), dropEnd.y);
		labelRect.left = min(min(min(origin.x, axisEnd.x), dropStart.x), dropEnd.x);
		labelRect.right = max(max(max(origin.x, axisEnd.x), dropStart.x), dropEnd.x);

		SelectObject(hdc, labelFont);
		DrawText(hdc, label.c_str(), label.size(), &labelRect, DT_CENTER | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE);

		SelectObject(hdc, oldFont);
		SelectObject(hdc, oldPen);
	}

	template<typename T>
	void Axis<T>::setMajorMinor() {
		/// TO DO: Implement
		major = 5;
		minor = 1;
	}
}

#include "axis.ipp"