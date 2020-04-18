#pragma once
#include <stdexcept>

namespace SimplePlot::Stats {
	template<typename T>
	T minValue(T* p, int size) {
		if (!p) {
			throw std::invalid_argument("p was nullptr");
		}
		T minSoFar = p[0];
		for (int i = 1; i < size; i++) {
			minSoFar = ((p[i] < minSoFar) ? p[i] : minSoFar);
		}
	}

	template<typename T>
	T maxValue(T* p, int size) {
		if (!p) {
			throw std::invalid_argument("p was nullptr");
		}
		T minSoFar = p[0];
		for (int i = 1; i < size; i++) {
			minSoFar = ((p[i] > minSoFar) ? p[i] : minSoFar);
		}
	}
}