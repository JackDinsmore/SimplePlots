#include "stats.h"
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

	template float minValue<float>(float* p, int size);
	template double minValue<double>(double* p, int size);
	template int minValue<int>(int* p, int size);


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

	template float maxValue<float>(float* p, int size);
	template double maxValue<double>(double* p, int size);
	template int maxValue<int>(int* p, int size);


	template<typename T>
	int binFindLeft(T* v, int size, T data, int start) {
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

	template int binFindLeft<float>(float* v, int size, float data, int start);
	template int binFindLeft<double>(double* v, int size, double data, int start);
	template int binFindLeft<int>(int* v, int size, int data, int start);


	template<typename T>
	std::wstring round(T f, int numDigs) {
		if (f == 0) { return L"0"; }
		int length = (int)log10(abs(f)) + 1 + numDigs;
		if (f < 0) { length++; }
		return std::to_wstring(f).substr(0, length);
	}

	template std::wstring round<float>(float f, int numDigs);
	template std::wstring round<double>(double f, int numDigs);
	template std::wstring round<int>(int f, int numDigs);
}