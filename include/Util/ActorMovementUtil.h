#pragma once

#include "JGeometry/TVec3.h"
#include "kamek.h"

class LiveActor;

namespace MR
{
    void makeMtxTR(Mtx, const LiveActor *);
    void makeMtxTRS(Mtx, const LiveActor*);

    void calcFrontVec(JGeometry::TVec3<f32> *, const LiveActor *);
    void calcUpVec(JGeometry::TVec3<f32> *, const LiveActor *);

    bool isOnPlayer(const HitSensor*);
    bool isNear(const LiveActor *, const JGeometry::TVec3<f32> &, f32);
    bool isNearPlayer(const LiveActor *, f32);
    bool isNearPlayerAnyTime(const LiveActor *, f32);

    bool isInWater(const LiveActor *, const JGeometry::TVec3<f32> &);
};