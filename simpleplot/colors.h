#pragma once
#include <windows.h>

#define SP_BLACK 0x0
#define SP_PURPLE 0x1
#define SP_GREEN 0x2
#define SP_BLUE 0x3

#define SP_WHITE 0x10// Not actually a color scheme

#define SP_SOLID 0x00
#define SP_DASH 0x10
#define SP_DOT 0x20
#define SP_DASHDOT 0x30
#define SP_DASHDOTDOT 0x40

#define SP_MEDIUM 0x000
#define SP_THIN 0x100
#define SP_THICK 0x200

namespace SimplePlot {

	namespace Style {

		typedef unsigned long SP_COLORREF;
		enum class Color {
			WHITE,
			BLACK,
			RED,
			YELLOW,
			LIGHT_GREEN,
			GREEN,
			LIGHT_BLUE,
			BLUE,
			TEAL,
			LIGHT_PURPLE,
			PURPLE,
			GRAY,
		};

		SP_COLORREF getColor(SimplePlot::Style::Color color);

		class Style {
		public:
			Style() {}
			Style(int);

			~Style();

		public:
			HBRUSH foreBrush = NULL;
			HBRUSH backBrush = NULL;
			HPEN forePen = NULL;
			HPEN backPen = NULL;
			int foreStyle;
		};
	}
}