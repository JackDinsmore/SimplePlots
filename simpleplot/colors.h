#pragma once

namespace SimplePlot {
	namespace Color {
		typedef unsigned long SP_COLORREF;

		enum class Color {
			WHITE,
			BLACK,
			RED,
			YELLOW,
			GREEN,
			BLUE,
			TEAL,
			PURPLE,
			GRAY,
		};

		SP_COLORREF getColor(SimplePlot::Color::Color color);
	}

	struct STYLE {
		Color::Color foreBrushColor;
		Color::Color backBrushColor;
		Color::Color forePenColor;
		Color::Color backPenColor;
	};

	namespace Style {
		const STYLE grayscale = { Color::Color::WHITE, Color::Color::WHITE, Color::Color::BLACK, Color::Color::GRAY};
	}
}