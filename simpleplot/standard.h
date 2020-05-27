#pragma once

#define SP_DEFAULT_WIDTH 800
#define SP_DEFAULT_HEIGHT 600
#define SP_DEFAULT_FRAMERATE 5
#define SP_DYNAMIC SP_DEFAULT_FRAMERATE
#define SP_STATIC -1
#define SP_BORDER_WIDTH 80
#define SP_TICK_LENGTH 6


namespace SimplePlot {
	typedef int PLOT_ID;
	typedef int CANVAS_ID;

	enum class PLOT_TYPE {
		// Ordered in terms of depth: the lowest numbers are drawn first.
		NULL_PLOT,
		LINE,
		SERIES,
		HISTOGRAM,
	};

	enum class AXIS_TYPE {
		NULL_AXES,
		CART_2D,
		CART_3D,
		SPECIAL,
	};
}