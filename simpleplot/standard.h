#pragma once

#include <windows.h>
#include <thread>
#include <mutex>
#include <string>
#include <map>
#include <functional>

#define SP_DEFAULT_WIDTH 800
#define SP_DEFAULT_HEIGHT 600
#define SP_DEFAULT_FRAMERATE 5
#define SP_DYNAMIC SP_DEFAULT_FRAMERATE
#define SP_STATIC -1
#define SP_BORDER_WIDTH 80


namespace SimplePlot {
	typedef int PLOT_ID;

	enum class PLOT_TYPE {
		NULL_PLOT,
		HISTOGRAM,
	};
}