#pragma once

#include "Actor/Nerve/NerveExecutor.h"

struct AnimScaleParam;

class AnimScaleController : public NerveExecutor
{
public:
    AnimScaleController(AnimScaleParam *);

    virtual ~AnimScaleController();
    void startHitReaction();

    u8 _0[8];
    u8 _8[4];
    JGeometry::TVec3<f32> mScale; // _C
    u8 _18[4];
};