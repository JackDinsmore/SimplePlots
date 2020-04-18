#pragma once

#include <windows.h>
#include <thread>
#include <mutex>
#include <string>
#include <map>
#include <functional>

#define SP_DEFAULT_WIDTH 800
#define SP_DEFAULT_HEIGHT 600
#define SP_FRAMERATE 5
#define SP_BUFFER_TOP 80
#define SP_BUFFER_RIGHT SP_BUFFER_TOP

typedef int PLOT_ID;