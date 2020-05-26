#pragma once
#include <windef.h>


namespace SimplePlot::Color {
	enum class Color {
		WHITE,
		BLACK,
		RED,
		YELLOW,
		GREEN,
		BLUE,
		TEAL,
		PURPLE,
	};


	inline COLORREF getColor(SimplePlot::Color::Color color) {
		switch(color) {
		case Color::WHITE:
			return RGB(255, 255, 255);
		case Color::BLACK:
			return RGB(0, 0, 0);
		case Color::RED:
			return RGB(255, 0, 0);
		case Color::YELLOW:
			return RGB(255, 255, 0);
		case Color::GREEN:
			return RGB(0, 255, 0);
		case Color::BLUE:
			return RGB(0, 0, 255);
		case Color::TEAL:
			return RGB(0, 255, 255);
		case Color::PURPLE:
			return RGB(255, 0, 255);
		default:
			return RGB(0, 0, 0);
		}
	}
}
