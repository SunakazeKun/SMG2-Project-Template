#include "spack/MapObj/Jiraira.h"
#include "Util.h"

/*
* Authors: Someone, Aurum
* Objects: Jiraira
* Parameters:
*  - Obj_arg1, float, 500.0f: Explosion range
*  - SW_AWAKE
* 
* The unused Bowser Land Mine from SMG1. This land mine explodes if the large button is touched.
* This can cause a chain reaction if multiple mines are placed next to each other.
*/

Jiraira::Jiraira(const char *pName) : LiveActor(pName) {
    mButtonCollision = NULL;
    mExplosionRange = 500.0f;
}

void Jiraira::init(const JMapInfoIter &rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneCollisionEnemy(this);

    // Initialize sensors
    initHitSensor(2);
    MR::getJMapInfoArg1NoInit(rIter, &mExplosionRange);
    TVec3f sensorOffset(0.0f, 30.0f, 0.0f);
    MR::addHitSensorMapObjMoveCollision(this, "Body", 16, 100.0f * mScale.y, sensorOffset);
    MR::addHitSensorEnemyAttack(this, "Explode", 16, mExplosionRange * mScale.y, sensorOffset);

    // Initialize collision
    MR::initCollisionPartsAutoEqualScaleOne(this, "Jiraira", getSensor("Body"), (MtxPtr)MR::getJointMtx(this, "Jiraira"));
    mButtonCollision = MR::createCollisionPartsFromLiveActor(this, "Button", getSensor("Body"), (MtxPtr)MR::getJointMtx(this, "Button"), MR::CollisionScaleType_1);
    MR::validateCollisionParts(mButtonCollision);

    initSound(6, "Jiraira", 0, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, NULL, false);
    MR::invalidateClipping(this);

    initNerve(&NrvJiraira::NrvWait::sInstance, NULL);

    MR::useStageSwitchAwake(this, rIter);
    appear();
}

void Jiraira::appear() {
    LiveActor::appear();
    MR::invalidateHitSensor(this, "Explode");
}

void Jiraira::control() {
    mButtonCollision->setMtx();
}

void Jiraira::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (pSender == getSensor("Explode")) {
        if (MR::isSensorPlayer(pReceiver))
            MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        else if (MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver)) {
            if (isNerve(&NrvJiraira::NrvSteppedExplode::sInstance))
                MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        }
    }
}

bool Jiraira::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    if (isNerve(&NrvJiraira::NrvExplode::sInstance) || isNerve(&NrvJiraira::NrvSteppedExplode::sInstance) || isNerve(&NrvJiraira::NrvPreRecover::sInstance))
        return false;

    if (MR::isMsgStarPieceAttack(msg)) {
        if (!isNerve(&NrvJiraira::NrvStepped::sInstance))
            setNerve(&NrvJiraira::NrvStepped::sInstance);
        return true;
    }
    return false;
}

bool Jiraira::receiveMsgEnemyAttack(u32 msg, HitSensor *, HitSensor *) {
    if (isNerve(&NrvJiraira::NrvExplode::sInstance) || isNerve(&NrvJiraira::NrvSteppedExplode::sInstance))
        return false;

    if (MR::isMsgExplosionAttack(msg)) {
        setNerve(&NrvJiraira::NrvExplode::sInstance);
        return true;
    }
    return false;
}

void Jiraira::exeWait() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "Wait");

    if (MR::isOnPlayer(getSensor("Body")))
        setNerve(&NrvJiraira::NrvStepped::sInstance);
}

void Jiraira::exeStepped() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Stepped");
        MR::tryRumblePadMiddle(this, NULL);
        MR::startLevelSound(this, "OjJirairaStepped", -1, -1, -1);
    }

    MR::startLevelSound(this, "OjLvJirairaCharge", -1, -1, -1);

    if (MR::isGreaterStep(this, 30))
        setNerve(&NrvJiraira::NrvSteppedExplode::sInstance);
}

void Jiraira::exeExplode() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Explosion");
        MR::startLevelSound(this, "OjJirairaExplode", -1, -1, -1);
        MR::startAction(this, "Down");
        MR::validateHitSensor(this, "Explode");
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }

    if (MR::isStep(this, 8))
        MR::invalidateHitSensor(this, "Explode");
    else if (MR::isLessStep(this, 8))
        getSensor("Explode")->mRadius = mExplosionRange * getNerveStep() * 0.125f;

    if (MR::isGreaterStep(this, 188))
        setNerve(&NrvJiraira::NrvPreRecover::sInstance);
}

void Jiraira::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startBrk(this, "RecoveryLoop");
    }

    MR::startLevelSound(this, "OjLvJirairaRecovering", -1, -1, -1);

    if (MR::isGreaterStep(this, 120))
        setNerve(&NrvJiraira::NrvRecover::sInstance);
}

void Jiraira::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recovery");
        MR::startLevelSound(this, "OjJirairaRecover", -1, -1, -1);
    }

    if (MR::isActionEnd(this))
        setNerve(&NrvJiraira::NrvWait::sInstance);
}

namespace NrvJiraira {
    void NrvWait::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeWait();
    }

    void NrvStepped::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeStepped();
    }

    void NrvPreRecover::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exePreRecover();
    }

    void NrvRecover::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeRecover();
    }

    void NrvSteppedExplode::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeExplode();
    }

    void NrvExplode::execute(Spine *pSpine) const {
        Jiraira* pActor = (Jiraira*)pSpine->mExecutor;
        pActor->exeExplode();
    }

    NrvWait(NrvWait::sInstance);
    NrvStepped(NrvStepped::sInstance);
    NrvPreRecover(NrvPreRecover::sInstance);
    NrvRecover(NrvRecover::sInstance);
    NrvSteppedExplode(NrvSteppedExplode::sInstance);
    NrvExplode(NrvExplode::sInstance);
}
