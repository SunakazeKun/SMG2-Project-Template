#pragma once

#include "LiveActor.h"

class SensorDetector : public LiveActor {
public:
    SensorDetector(const char *pName);

    virtual void init(const JMapInfoIter &rIter);
    virtual void control();
    virtual bool receiveMessage(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

    s32 mMessage;
    f32 mHitbox;
    bool mNoRumble;
};
