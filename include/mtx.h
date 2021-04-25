#pragma once

#include "kamek.h"
#include "JGeometry/TMatrix34.h"

#ifdef __cplusplus
extern "C" {
#endif

void PSMTXIdentity(Mtx matrix);
void PSMTXCopy(const Mtx sourceMtx, Mtx destMtx);
void PSMTXMultVec(const Mtx matrix, const JGeometry::TVec3<f32>* source, JGeometry::TVec3<f32>* dest);
void PSMTXConcat(const Mtx sourceA, const Mtx sourceB, Mtx dest);

f32 PSVECMag(const Vec *);

#ifdef __cplusplus
}
#endif