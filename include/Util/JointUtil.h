#pragma once

#include "kamek.h"
#include "JGeometry/TMatrix34.h"

class LiveActor;

namespace MR
{
    JGeometry::TMatrix34<f32>* getJointMtx(const LiveActor *, s32);

    JGeometry::TMatrix34<f32>* getJointMtx(const LiveActor *, const char *);

    void copyJointPos(const LiveActor*, const char*, JGeometry::TVec3<f32>*);
};