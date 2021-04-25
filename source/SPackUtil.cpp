#include "spack/SPackUtil.h"

void SPack::clamp(s32 min, s32 max, s32* val) {
	if (max < min) {
		s32 tmp = max;
		max = min;
		min = max;
	}

	if (*val < min)
		*val = min;
	else if (*val > max)
		*val = max;
}
