#include "colors.h"
#include <Windows.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

namespace SimplePlot::Style {
	SP_COLORREF getColor(SimplePlot::Style::Color color) {
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

	Style::Style(int s) {
		COLORREF forePenColor;
		COLORREF backPenColor;
		COLORREF foreBrushColor;
		COLORREF backBrushColor;
		switch (s % 16) {
		default:
		case SP_BLACK:
			forePenColor = getColor(Color::BLACK);
			backPenColor = getColor(Color::GRAY);
			foreBrushColor = getColor(Color::WHITE);
			backBrushColor = getColor(Color::WHITE);
			break;
		case SP_PURPLE:
			forePenColor = getColor(Color::PURPLE);
			backPenColor = getColor(Color::GRAY);
			foreBrushColor = getColor(Color::LIGHT_PURPLE);
			backBrushColor = getColor(Color::WHITE);
			break;
		case SP_GREEN:
			forePenColor = getColor(Color::GREEN);
			backPenColor = getColor(Color::GRAY);
			foreBrushColor = getColor(Color::LIGHT_GREEN);
			backBrushColor = getColor(Color::WHITE);
			break;
		case SP_BLUE:
			forePenColor = getColor(Color::BLUE);
			backPenColor = getColor(Color::GRAY);
			foreBrushColor = getColor(Color::LIGHT_BLUE);
			backBrushColor = getColor(Color::WHITE);
			break;
		}

		int style;
		switch (((s / 16) % 16) * 16) {
		default:
		case SP_SOLID:
			style = PS_SOLID; break;
		case SP_DASH:
			style = PS_DASH; break;
		case SP_DOT:
			style = PS_DOT; break;
		case SP_DASHDOT:
			style = PS_DASHDOT; break;
		case SP_DASHDOTDOT:
			style = PS_DASHDOTDOT; break;
		}

		int thickness;
		switch (((s / 256) % 16) * 256) {
		case SP_THIN:
			thickness = 1; break;
		default:
		case SP_MEDIUM:
			thickness = 2; break;
		case SP_THICK:
			thickness = 3; break;
		}

		foreBrush = CreateSolidBrush(foreBrushColor);
		backBrush = CreateSolidBrush(backBrushColor);
		forePen = CreatePen(style, thickness, forePenColor);
		backPen = CreatePen(PS_SOLID, 2, backPenColor);
		foreStyle = ((s / 16) % 16) * 16;
	}

	Style::~Style() {
		if (foreBrush != NULL) {
			DeleteObject(foreBrush);
			DeleteObject(backBrush);
			DeleteObject(forePen);
			DeleteObject(backPen);
		}
	}
}