#pragma once
#include <string>

namespace SimplePlot::Stats {
	template<typename T>
	T minValue(T* p, int size);

	template<typename T>
	T maxValue(T* p, int size);

	template<typename T>
	int binFindLeft(T* v, int size, T data, int start = 0);

	template<typename T>
	std::wstring round(T f, int numDigs);
}