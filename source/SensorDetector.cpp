#include "spack/Actor/SensorDetector.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"

SensorDetector::SensorDetector(const char* pName) : LiveActor(pName) {
    mMessage = 0;
    mHitbox = 100.0f;
    mNoRumble = false;
}

void SensorDetector::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);

    MR::getJMapInfoArg0NoInit(rIter, &mMessage);
    MR::getJMapInfoArg1NoInit(rIter, &mHitbox);
    MR::getJMapInfoArg2NoInit(rIter, &mNoRumble);
    MR::useStageSwitchWriteDead(this, rIter);

    initHitSensor(1);
    MR::addHitSensor(this, "Body", 0, 1, mHitbox, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

    makeActorAppeared();
}

void SensorDetector::control() {
    if (!mNoRumble && MR::isNearPlayerAnyTime(this, mHitbox + 50.0f))
        MR::tryRumblePadMiddle(this, 0);
}

s32 SensorDetector::receiveMessage(u32 msg, HitSensor* pHitA, HitSensor* pHitB) {
    if (msg == mMessage) {
        MR::onSwitchDead(this);
        makeActorDead();
    }

    return 0;
}
