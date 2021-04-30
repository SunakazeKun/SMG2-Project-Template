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

void SPack::clamp(f32 min, f32 max, f32* val) {
	if (max < min) {
		f32 tmp = max;
		max = min;
		min = max;
	}

	if (*val < min)
		*val = min;
	else if (*val > max)
		*val = max;
}
