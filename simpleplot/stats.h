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
		return minSoFar;
	}

	template<typename T>
	T maxValue(T* p, int size) {
		if (!p) {
			throw std::invalid_argument("p was nullptr");
		}
		T maxSoFar = p[0];
		for (int i = 1; i < size; i++) {
			maxSoFar = ((p[i] > maxSoFar) ? p[i] : maxSoFar);
		}
		return maxSoFar;
	}

	template<typename T>
	int binFindLeft(T* v, int size, T data, int start=0) {
		// Find the left point of an interval in v that contains data.
		if (size == 1) {
			if (v[0] <= data) {
				return start;
			}
			else {
				if (start > 0) {
					return start - 1;
				}
				else {
					std::invalid_argument("List was too small");
				}
			}
		}

		if (v[size / 2] == data) {
			return size / 2;
		}
		else if (v[size / 2] < data) {
			// Search right
			return binFindLeft(v, size - (size / 2 + 1), data, start + size / 2 + 1);
		}
		else {
			// Search left
			return binFindLeft(v, size / 2, data, start);
		}
	}

	template<typename T>
	std::wstring round(T f, int numDigs) {
		return std::to_wstring(f);
	}
}