#include "spack/Actor/Jiraira.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/EffectUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SoundUtil.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

Jiraira::Jiraira(const char* pName) : LiveActor(pName) {
    // Instantiate nerves
    for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
        (*f)();

    mExplosionRange = 500.0f;
}

void Jiraira::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Jiraira", NULL);
    MR::connectToSceneCollisionEnemy(this);

    MR::getJMapInfoArg1NoInit(rIter, &mExplosionRange);
    MR::useStageSwitchWriteDead(this, rIter);

    initHitSensor(2);
    MR::addHitSensorMapObjMoveCollision(this, "Body", 16, 100.0f * mScale.y, JGeometry::TVec3<f32>(0.0f, 30.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "Explode", 16, mExplosionRange * mScale.y, JGeometry::TVec3<f32>(0.0f, 30.0f, 0.0f));

    MR::initCollisionParts(this, "Button", MR::getSensor(this, "Body"), NULL);
    MR::validateCollisionParts(this);

    initSound(6, "Jiraira", 0, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, NULL, false);
    initNerve(&NrvJiraira::NrvWait::sInstance, NULL);
    MR::invalidateClipping(this);

    makeActorAppeared();
}

void Jiraira::kill() {
    LiveActor::kill();

    if (MR::isValidSwitchDead(this))
        MR::onSwitchDead(this);
}

void Jiraira::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
    if (pHit1 == MR::getSensor(this, "Explode") && (MR::isSensorPlayer(pHit2) || MR::isSensorEnemy(pHit2) || MR::isSensorMapObj(pHit2)))
        MR::sendMsgEnemyAttackExplosion(pHit2, pHit1);
}

u32 Jiraira::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
    if (isNerve(&NrvJiraira::NrvExplode::sInstance) || isNerve(&NrvJiraira::NrvPreRecover::sInstance) || isNerve(&NrvJiraira::NrvRecover::sInstance))
        return 0;

    if (MR::isMsgStarPieceAttack(msg)) {
        if (!isNerve(&NrvJiraira::NrvStepped::sInstance))
            setNerve(&NrvJiraira::NrvStepped::sInstance);
        return 1;
    }
    return 0;
}

u32 Jiraira::receiveMsgEnemyAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
    if (isNerve(&NrvJiraira::NrvExplode::sInstance) || isNerve(&NrvJiraira::NrvPreRecover::sInstance))
        return 0;

    if (MR::isMsgExplosionAttack(msg)) {
        setNerve(&NrvJiraira::NrvStepped::sInstance);
        return 1;
    }
    return 0;
}

void Jiraira::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::startBrk(this, "Wait");
        MR::invalidateHitSensor(this, "Explode");
    }

    if (MR::isOnPlayer(MR::getSensor(this, "Body")))
        setNerve(&NrvJiraira::NrvStepped::sInstance);
}

void Jiraira::exeStepped() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stepped");
        MR::startBrk(this, "Stepped");
        MR::tryRumblePadMiddle(this, NULL);
        MR::startLevelSound(this, "OjJirairaStepped", -1, -1, -1);
    }

    MR::startLevelSound(this, "OjLvJirairaCharge", -1, -1, -1);

    if (MR::isGreaterStep(this, 30))
        setNerve(&NrvJiraira::NrvExplode::sInstance);
}

void Jiraira::exeExplode() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Explosion");
        MR::startLevelSound(this, "OjJirairaExplode", -1, -1, -1);
        MR::startBck(this, "Down");
        MR::startBrk(this, "Down");
        MR::validateHitSensor(this, "Explode");
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }

    if (MR::isStep(this, 8))
        MR::invalidateHitSensor(this, "Explode");
    else if (MR::isLessStep(this, 8))
        getSensor("Explode")->_10 = mExplosionRange * getNerveStep() * 0.125f;

    if (MR::isGreaterStep(this, 188))
        setNerve(&NrvJiraira::NrvPreRecover::sInstance);
}

void Jiraira::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down");
        MR::startBrk(this, "RecoveryLoop");
    }

    MR::startLevelSound(this, "OjLvJirairaRecovering", -1, -1, -1);

    if (MR::isGreaterStep(this, 120))
        setNerve(&NrvJiraira::NrvRecover::sInstance);
}

void Jiraira::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recovery");
        MR::startBrk(this, "Recovery");
        MR::startLevelSound(this, "OjJirairaRecover", -1, -1, -1);
    }

    if (MR::isActionEnd(this))
        setNerve(&NrvJiraira::NrvWait::sInstance);
}

namespace NrvJiraira {
    void NrvWait::execute(Spine* pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeWait();
    }

    void NrvStepped::execute(Spine* pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeStepped();
    }

    void NrvExplode::execute(Spine* pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeExplode();
    }

    void NrvPreRecover::execute(Spine* pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exePreRecover();
    }

    void NrvRecover::execute(Spine* pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeRecover();
    }

    NrvWait(NrvWait::sInstance);
    NrvStepped(NrvStepped::sInstance);
    NrvExplode(NrvExplode::sInstance);
    NrvPreRecover(NrvPreRecover::sInstance);
    NrvRecover(NrvRecover::sInstance);
}
