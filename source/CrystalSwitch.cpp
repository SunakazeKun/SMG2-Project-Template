#include "spack/Actor/CrystalSwitch.h"
#include "spack/SPackUtil.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"
#include "Util/StarPointerUtil.h"
#include "Util/SupportTicoUtil.h"

/*
* Created by Evanbowl & Someone with help by Aurum
* 
* CrystalSwitch is one of SMG1's many unused switch controllers. It has a unique crystal design not
* seen anywhere else in the game. In SMG1 the event is always activated and the crystal spins won't
* stop spinning once activated. However, unused code suggests that it was planned to make it stop
* spinning and deactivate the event again. This removed behavior has been restored here and can be
* utilized with Obj_arg0.
*/
CrystalSwitch::CrystalSwitch(const char* pName) : LiveActor(pName) {
    mActiveTime = -1;
    mRotSpeed = 0.0f;
    mStartSpin = false;
}

void CrystalSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("CrystalSwitch", NULL);
    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    TVec3f offSensor(0.0f, 100.0f, 0.0f);
    MR::addHitSensorMapObj(this, "Body", 16, 100.0f, offSensor);
    MR::initStarPointerTarget(this, 70.0f, offSensor);

    initSound(1, "CrystalSwitch", false, TVec3f(0.0f, 0.0f, 0.0f));
    MR::useStageSwitchWriteA(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mActiveTime);
    initNerve(&NrvCrystalSwitch::NrvOff::sInstance, 0);

    MR::startAction(this, "Wait");
    MR::startBpk(this, "Off");

    makeActorAppeared();
}

void CrystalSwitch::control() {
    calcRotSpeed();
    mStartSpin = false;
    MR::attachSupportTicoToTarget(this);
}

void CrystalSwitch::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
    if (MR::isSensorPlayer(pHit2))
        MR::sendMsgPush(pHit2, pHit1);
}

u32 CrystalSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pHitA, HitSensor* pHitB) {
    if (MR::isMsgPlayerHitAll(msg) || MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerTrample(msg) || MR::isMsgStarPieceReflect(msg)) {
        mStartSpin = true;
        return !MR::isMsgPlayerHitAll(msg);
    }

    return 0;
}

void CrystalSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offSwitchA(this);
    }

    if (mStartSpin)
        setNerve(&NrvCrystalSwitch::NrvSwitchDown::sInstance);
}

void CrystalSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBpk(this, "On");
        MR::invalidateClipping(this);
    }

    if (MR::isGreaterStep(this, 10))
        setNerve(&NrvCrystalSwitch::NrvOn::sInstance);
}

void CrystalSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::onSwitchA(this);
        MR::shakeCameraNormal();
        MR::startLevelSound(this, "OjCrystalSwitchOn", -1, -1, -1);
        mRotSpeed = 0.0f;
    }

    // This behavior is not implemented in SMG1
    if (mActiveTime > -1 && MR::isStep(this, mActiveTime))
        setNerve(&NrvCrystalSwitch::NrvSwitchUp::sInstance);
}

// Never called in SMG1, making this behavior effectively unused in that game
void CrystalSwitch::exeSwitchUp() {
    if (MR::isFirstStep(this))
        MR::startBpk(this, "Off");

    if (mRotSpeed >= 8.0f)
        setNerve(&NrvCrystalSwitch::NrvOff::sInstance);
}

void CrystalSwitch::calcRotSpeed() {
    // Switch gets activated
    if (isNerve(&NrvCrystalSwitch::NrvOn::sInstance)) {
        if (mRotSpeed < 10.0f)
            mRotSpeed += 1.5f;
        if (mRotSpeed > 10.0f)
            mRotSpeed = 10.0f;

        MR::setBckRate(this, mRotSpeed);
    }
    // Switch gets deactivated
    else {
        if (mRotSpeed > 10.0f)
            mRotSpeed = 10.0f;
        MR::setBckRate(this, mRotSpeed);

        mRotSpeed *= 0.99000001f;
        if (mRotSpeed < 0.1f)
            mRotSpeed = 0.0f;
    }
}

namespace NrvCrystalSwitch {
    void NrvOff::execute(Spine* pSpine) const {
        CrystalSwitch* pActor = (CrystalSwitch*)pSpine->mExecutor;
        pActor->exeOff();
    }

    void NrvSwitchDown::execute(Spine* pSpine) const {
        CrystalSwitch* pActor = (CrystalSwitch*)pSpine->mExecutor;
        pActor->exeSwitchDown();
    }

    void NrvOn::execute(Spine* pSpine) const {
        CrystalSwitch* pActor = (CrystalSwitch*)pSpine->mExecutor;
        pActor->exeOn();
    }

    void NrvSwitchUp::execute(Spine* pSpine) const {
        CrystalSwitch* pActor = (CrystalSwitch*)pSpine->mExecutor;
        pActor->exeSwitchUp();
    }

    NrvOff(NrvOff::sInstance);
    NrvSwitchDown(NrvSwitchDown::sInstance);
    NrvOn(NrvOn::sInstance);
    NrvSwitchUp(NrvSwitchUp::sInstance);
}
