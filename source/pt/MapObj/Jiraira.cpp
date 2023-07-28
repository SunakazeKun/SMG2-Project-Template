#include "pt/MapObj/Jiraira.h"

/*
* Authors: Someone, Aurum
* Objects: Jiraira
* Parameters:
*  - SW_AWAKE
* 
* The unused Bowser Land Mine from SMG1. This land mine explodes if the large button is touched. This can cause a chain
* reaction if multiple mines are placed next to each other.
*/

namespace pt {
    Jiraira::Jiraira(const char *pName) : LiveActor(pName) {
        mButtonCollision = NULL;
    }

    void Jiraira::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        MR::initDefaultPos(this, rIter);
        MR::connectToSceneCollisionEnemy(this);

        // Initialize sensors
        initHitSensor(2);
        TVec3f sensorOffset(0.0f, 30.0f, 0.0f);
        MR::addHitSensorMapObjMoveCollision(this, "Body", 16, 100.0f * mScale.y, sensorOffset);
        MR::addHitSensorEnemyAttack(this, "Explode", 16, 500.0f * mScale.y, sensorOffset);

        // Initialize collision
        HitSensor *senBody = getSensor("Body");
        MR::initCollisionPartsAutoEqualScaleOne(this, "Jiraira", senBody, MR::getJointMtx(this, "Jiraira"));
        mButtonCollision = MR::createCollisionPartsFromLiveActor(this, "Button", senBody, MR::getJointMtx(this, "Button"), MR::CollisionScaleType_NotUsingScale);
        MR::validateCollisionParts(mButtonCollision);

        // Initialize miscellaneous attributes
        initSound(6, "Jiraira", NULL, TVec3f(0.0f, 0.0f, 0.0f));
        initEffectKeeper(0, NULL, false);
        MR::invalidateClipping(this);
        MR::useStageSwitchAwake(this, rIter);

        // Setup nerve and make appeared
        initNerve(&NrvJiraira::NrvWait::sInstance, NULL);
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
            if (MR::isSensorPlayerOrRide(pReceiver)) {
                MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
            }
            else if (MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver)) {
                if (isNerve(&NrvJiraira::NrvSteppedExplode::sInstance)) {
                    MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
                }
            }
        }
    }

    bool Jiraira::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (isNerve(&NrvJiraira::NrvExplode::sInstance)
            || isNerve(&NrvJiraira::NrvSteppedExplode::sInstance)
            || isNerve(&NrvJiraira::NrvPreRecover::sInstance))
        {
            return false;
        }

        if (MR::isMsgStarPieceAttack(msg)) {
            if (!isNerve(&NrvJiraira::NrvStepped::sInstance)) {
                setNerve(&NrvJiraira::NrvStepped::sInstance);
            }
            return true;
        }

        return false;
    }

    bool Jiraira::receiveMsgEnemyAttack(u32 msg, HitSensor *, HitSensor *) {
        if (isNerve(&NrvJiraira::NrvExplode::sInstance) || isNerve(&NrvJiraira::NrvSteppedExplode::sInstance)) {
            return false;
        }

        if (MR::isMsgExplosionAttack(msg)) {
            setNerve(&NrvJiraira::NrvExplode::sInstance);
            return true;
        }

        return false;
    }

    void Jiraira::exeWait() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Wait");
        }

        // !MR::isPlayerFlying() needed to prevent Flying Mario from triggering the explosion without touching it.
        if (MR::isOnPlayer(getSensor("Body")) && !MR::isPlayerFlying()) {
            setNerve(&NrvJiraira::NrvStepped::sInstance);
        }
    }

    void Jiraira::exeStepped() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Stepped");
            MR::tryRumblePadMiddle(this, NULL);
            MR::startActionSound(this, "OjJirairaStepped", -1, -1, -1);
        }

        MR::startActionSound(this, "OjLvJirairaCharge", -1, -1, -1);

        if (MR::isGreaterStep(this, 30)) {
            setNerve(&NrvJiraira::NrvSteppedExplode::sInstance);
        }
    }

    void Jiraira::exeExplode() {
        if (MR::isFirstStep(this)) {
            MR::emitEffect(this, "Explosion");
            MR::startActionSound(this, "OjJirairaExplode", -1, -1, -1);
            MR::startAction(this, "Down");
            MR::validateHitSensor(this, "Explode");
            MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
        }

        if (MR::isStep(this, 8)) {
            MR::invalidateHitSensor(this, "Explode");
        }
        else if (MR::isLessStep(this, 8)) {
            getSensor("Explode")->mRadius = 500.0f * getNerveStep() * 0.125f;
        }

        if (MR::isGreaterStep(this, 188)) {
            setNerve(&NrvJiraira::NrvPreRecover::sInstance);
        }
    }

    void Jiraira::exePreRecover() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Down");
            MR::startBrk(this, "RecoveryLoop");
        }

        MR::startActionSound(this, "OjLvJirairaRecovering", -1, -1, -1);

        if (MR::isGreaterStep(this, 120)) {
            setNerve(&NrvJiraira::NrvRecover::sInstance);
        }
    }

    void Jiraira::exeRecover() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Recovery");
            MR::startActionSound(this, "OjJirairaRecover", -1, -1, -1);
        }

        if (MR::isActionEnd(this)) {
            setNerve(&NrvJiraira::NrvWait::sInstance);
        }
    }

    namespace NrvJiraira {
        void NrvWait::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exeWait();
        }

        void NrvStepped::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exeStepped();
        }

        void NrvPreRecover::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exePreRecover();
        }

        void NrvRecover::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exeRecover();
        }

        void NrvSteppedExplode::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exeExplode();
        }

        void NrvExplode::execute(Spine *pSpine) const {
            Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
            pActor->exeExplode();
        }

        NrvWait(NrvWait::sInstance);
        NrvStepped(NrvStepped::sInstance);
        NrvPreRecover(NrvPreRecover::sInstance);
        NrvRecover(NrvRecover::sInstance);
        NrvSteppedExplode(NrvSteppedExplode::sInstance);
        NrvExplode(NrvExplode::sInstance);
    }
}
