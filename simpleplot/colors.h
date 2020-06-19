#pragma once

namespace SimplePlot {
	namespace Color {
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

		SP_COLORREF getColor(SimplePlot::Color::Color color);
	}

	struct STYLE {
		Color::Color foreBrushColor;
		Color::Color backBrushColor;
		Color::Color forePenColor;
		Color::Color backPenColor;
	};

	namespace Style {
		const STYLE grayscale = { Color::Color::WHITE, Color::Color::WHITE, Color::Color::BLACK, Color::Color::GRAY };
		const STYLE purple = { Color::Color::LIGHT_PURPLE, Color::Color::WHITE, Color::Color::PURPLE, Color::Color::GRAY };
		const STYLE green = { Color::Color::LIGHT_GREEN, Color::Color::WHITE, Color::Color::GREEN, Color::Color::GRAY };
		const STYLE blue = { Color::Color::LIGHT_BLUE, Color::Color::WHITE, Color::Color::BLUE, Color::Color::GRAY };
	}
}