#include "spack/Actor/PomponPlant.h"
#include "spack/SPackUtil.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SoundUtil.h"
#include "Util/StarPointerUtil.h"
#include "Util/SupportTicoUtil.h"

/*
* Created by Someone with help by Aurum
* 
* This is the Bubbly Plant from Honeyhive galaxy. When touched, it spawns Coins or Star Bits. This
* also has new P2 functionality here.
*/
PomponPlant::PomponPlant(const char* pActorName) : LiveActor(pActorName) {
    mCollectible = -1;
    mAmount = 1;
    mItemGenerated = false;
}

void PomponPlant::init(const JMapInfoIter &rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);

    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(objName, NULL);
    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    TVec3f offSensor(55.0f, 170.0f, -100.0f);
    MR::addHitSensorMapObj(this, "Body", 1, 70.0f, offSensor);
    MR::initStarPointerTarget(this, 70.0f, offSensor);

    initEffectKeeper(0, objName, false);
    initSound(2, "PomponPlant", 0, TVec3f(0.0f, 0.0f, 0.0f));

    MR::getJMapInfoArg0NoInit(rIter, &mCollectible);
    MR::getJMapInfoArg1NoInit(rIter, &mAmount);

    if (mCollectible == 0)
        MR::declareCoin(this, mAmount);
    else if (mCollectible == 1)
        MR::declareStarPiece(this, mAmount);

    initNerve(&NrvPomponPlant::NrvWait::sInstance, 0);

    makeActorAppeared();
}

void PomponPlant::control() {
    MR::attachSupportTicoToTarget(this);
}

void PomponPlant::attackSensor(HitSensor *pHit1, HitSensor *pHit2) {
    if (MR::isSensorPlayer(pHit2) && !isNerve(&NrvPomponPlant::NrvSwingPlayer::sInstance)) {
        if (MR::sendMsgTouchJump(pHit2, pHit1))
            setNerve(&NrvPomponPlant::NrvSwingPlayer::sInstance);
    }
}

u32 PomponPlant::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    if (MR::isMsgSupportTicoSpin(msg))
        setNerve(&NrvPomponPlant::NrvSwingPlayer::sInstance);
    else if (MR::isMsgStarPieceReflect(msg)) {
        setNerve(&NrvPomponPlant::NrvSwingPlayer::sInstance);
        return 1;
    }

    return 0;
}

bool PomponPlant::tryGenItem() {
    if (mItemGenerated)
        return false;

    if (mCollectible == 0)
        MR::appearCoinPop(this, getSensor("body")->_4, mAmount);
    else if (mCollectible == 1) {
        if (MR::appearStarPiece(this, getSensor("body")->_4, 10.0f, 40.0f, mAmount, false))
            MR::startLevelSound(this, "OjStarPieceBurst", -1, -1, -1);
    }

    mItemGenerated = true;
    return true;
};

void PomponPlant::exeSwingDpd() {
    if (MR::isFirstStep(this)) {
        MR::startLevelSound(this, "OjPomponPlantDpdHit", -1, -1, -1);
        MR::startAction(this, "Dpdhit");
    }

    if (MR::isActionEnd(this))
        setNerve(&NrvPomponPlant::NrvWait::sInstance);
}

void PomponPlant::exeSwingPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startLevelSound(this, "OjPomponPlantHit", -1, -1, -1);
        MR::startAction(this, "Hit");
        tryGenItem();
    }

    if (MR::isActionEnd(this))
        setNerve(&NrvPomponPlant::NrvWait::sInstance);
}

namespace NrvPomponPlant {
    void NrvSwingPlayer::execute(Spine* pSpine) const {
        PomponPlant* pActor = (PomponPlant*)pSpine->mExecutor;
        pActor->exeSwingPlayer();
    }

    void NrvSwingDpd::execute(Spine* pSpine) const {
        PomponPlant* pActor = (PomponPlant*)pSpine->mExecutor;
        pActor->exeSwingDpd();
    }

    void NrvWait::execute(Spine* pSpine) const {
        PomponPlant* pActor = (PomponPlant*)pSpine->mExecutor;

        if (MR::tryStarPointerCheckWithoutRumble(pActor, 0))
            pActor->setNerve(&NrvSwingDpd::sInstance);
    }

    NrvWait(NrvWait::sInstance);
    NrvSwingPlayer(NrvSwingPlayer::sInstance);
    NrvSwingDpd(NrvSwingDpd::sInstance);
};
