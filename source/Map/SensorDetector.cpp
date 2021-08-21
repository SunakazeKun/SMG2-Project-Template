#include "spack/Map/SensorDetector.h"
#include "Util.h"

/*
* Authors: Someone, Evanbowl
* Objects: SensorDetector
* Parameters:
*  - Obj_arg0, long, 0: Message type
*  - Obj_arg1, float, 100.0f: Sensor radius
*  - Obj_arg2, bool, false: Disable rumble
*  - SW_DEAD, need: Activated after detecting message
* 
* This actor detects if the specified message is in the sensor's range. If that's the case, its
* SW_DEAD will be activated. It can also be set to rumble the Wiimote if the player is nearby.
*/

SensorDetector::SensorDetector(const char *pName) : LiveActor(pName) {
    mMessage = 0;
    mHitbox = 100.0f;
    mNoRumble = false;
}

void SensorDetector::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);

    MR::getJMapInfoArg0NoInit(rIter, &mMessage);
    MR::getJMapInfoArg1NoInit(rIter, &mHitbox);
    MR::getJMapInfoArg2NoInit(rIter, &mNoRumble);
    MR::needStageSwitchWriteDead(this, rIter);

    initHitSensor(1);
    MR::addHitSensor(this, "Body", ATYPE_RECEIVER, 1, mHitbox, TVec3f(0.0f, 0.0f, 0.0f));

    makeActorAppeared();
}

void SensorDetector::control() {
    if (!mNoRumble && MR::isNearPlayerAnyTime(this, mHitbox + 50.0f))
        MR::tryRumblePadMiddle(this, 0);
}

bool SensorDetector::receiveMessage(u32 msg, HitSensor *, HitSensor *) {
    if (msg == mMessage) {
        MR::onSwitchDead(this);
        makeActorDead();
    }

    return false;
}
