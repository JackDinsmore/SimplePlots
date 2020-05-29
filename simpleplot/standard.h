#pragma once

#define SP_DEFAULT_WIDTH 800
#define SP_DEFAULT_HEIGHT 600
#define SP_DEFAULT_FRAMERATE 5
#define SP_DYNAMIC SP_DEFAULT_FRAMERATE
#define SP_STATIC -1
#define SP_BORDER_WIDTH 80
#define SP_TICK_LENGTH 6
#define SP_NULL_PLOT -1
#define SP_NULL_CANVAS -1
#define SP_X_AXIS 0
#define SP_Y_AXIS 1
#define SP_Z_AXIS 2


namespace SimplePlot {
	typedef int PLOT_ID;
	typedef int CANVAS_ID;

	enum class PLOT_TYPE {
		// Ordered in terms of depth: the lowest numbers are drawn first.
		SP_NULL_PLOT_TYPE,
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