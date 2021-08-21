#pragma once

namespace SPack {
	template<typename T>
	void clamp(T min, T max, T* val) {
		if (max < min) {
			T tmp = max;
			max = min;
			min = tmp;
		}

		if (*val < min)
			*val = min;
		else if (*val > max)
			*val = max;
	}
};
