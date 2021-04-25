#pragma once

#include "kamek.h"

#ifdef __cplusplus
extern "C" {
#endif

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);

#ifdef __cplusplus
}
#endif

namespace JMath {
	void gekko_ps_copy12(void*, const void*);
}
