#pragma once

namespace pt {
	template<typename T>
	void clamp(T min, T max, T* val) {
		if (*val < min)
			*val = min;
		else if (*val > max)
			*val = max;
	}
};
