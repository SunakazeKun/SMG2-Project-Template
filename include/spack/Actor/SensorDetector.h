#pragma once

#include "Actor/LiveActor/LiveActor.h"

class SensorDetector : public LiveActor {
public:
    SensorDetector(const char*);

    virtual void init(const JMapInfoIter& Iter);
    virtual void control();
    virtual s32 receiveMessage(u32, HitSensor*, HitSensor*);

    s32 mMessage;
    f32 mHitbox;
    bool mNoRumble;
};
