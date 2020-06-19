#include "colors.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define RGB(r,g,b) ((SimplePlot::Color::SP_COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

namespace SimplePlot::Color {
	SP_COLORREF getColor(SimplePlot::Color::Color color) {
		switch (color) {
		case Color::WHITE:
			return RGB(255, 255, 255);
		case Color::BLACK:
			return RGB(0, 0, 0);
		case Color::RED:
			return RGB(255, 0, 0);
		case Color::YELLOW:
			return RGB(255, 255, 0);
		case Color::LIGHT_GREEN:
			return RGB(128, 255, 128);
		case Color::GREEN:
			return RGB(0, 255, 0);
		case Color::LIGHT_BLUE:
			return RGB(128, 128, 255);
		case Color::BLUE:
			return RGB(0, 0, 255);
		case Color::TEAL:
			return RGB(0, 255, 255);
		case Color::LIGHT_PURPLE:
			return RGB(255, 128, 255);
		case Color::PURPLE:
			return RGB(255, 0, 255);
		case Color::GRAY:
			return RGB(128, 128, 128);
		default:
			return RGB(0, 0, 0);
		}
	}
}