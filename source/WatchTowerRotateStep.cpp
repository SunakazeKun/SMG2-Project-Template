#include "spack/Actor/WatchTowerRotateStep.h"
#include "spack/SPackUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/DemoUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/MathUtil.h"
#include "Util/MtxUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"

/*
* Created by Someone with help by shibboleet and Aurum
* 
* The windmill from Honeyhive is a unique kind of platform object. Unlike SMG1, the rotation speed
* can be changed now.
*/
WatchTowerRotateStep::WatchTowerRotateStep(const char* pName) : LiveActor(pName) {
    mRotSpeed = 300.0f;
}

void WatchTowerRotateStep::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WatchTowerRotateStep", NULL);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);

    HitSensor* senBody = MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "WatchTowerRotateStep", senBody, NULL);
    //initEffectKeeper(0, NULL, false);

    initSound(4, "WatchTowerRotateStep", false, TVec3f(0.0f, 0.0f, 0.0f));
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 1500.0f);

    bool registerDemoCast = MR::tryRegisterDemoCast(this, rIter);
    if (registerDemoCast)
        MR::registerDemoActionNerve(this, &WatchTowerRotateStepNrv::NrvMoveStart::sInstance, NULL);

    MR::getJMapInfoArg0NoInit(rIter, &mRotSpeed);
    mRotSpeed /= 1000.0f;

    MR::calcUpVec(&mRotateDeg, this);
    initLift(rIter);

    if (registerDemoCast)
        initNerve(&WatchTowerRotateStepNrv::NrvWait::sInstance, 0);
    else
        initNerve(&WatchTowerRotateStepNrv::NrvMove::sInstance, 0);

    makeActorAppeared();
}

void WatchTowerRotateStep::calcAndSetBaseMtx() {
    TVec3f mUpVec;
    TPositionMtx mFrontUp;

    MR::calcFrontVec(&mUpVec, this);
    MR::makeMtxFrontUpPos(&mFrontUp, mUpVec, mRotateDeg, mTranslation);
    MR::setBaseTRMtx(this, mFrontUp);
}

void WatchTowerRotateStep::initLift(const JMapInfoIter& rIter) {
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TMatrix34<f32>* mtx = MR::getJointMtx(this, i + 1);
        PartsModel* platform = new PartsModel(this, "物見の塔リフト", "WatchTowerRotateStepLift", (Mtx4*)mtx, -1, 0);
        mParts[i] = platform;

        MR::initCollisionParts(platform, "WatchTowerRotateStepLift", getSensor("Body"), NULL);

        platform->initShadowControllerList(1);
        MR::addShadowVolumeBox(platform, "Body", TVec3f(600.0f, 200.0f, 400.0f), *platform->getBaseMtx());
        MR::setShadowVolumeStartDropOffset(platform, "WatchTowerRotateStepLift", 300.0f);
        MR::setShadowDropLength(platform, "WatchTowerRotateStepLift", 370.0f);

        platform->_9C = 0;

        if (MR::isDemoCast(this, NULL))
            MR::tryRegisterDemoCast(platform, rIter);

        platform->initWithoutIter();
    }
}

void WatchTowerRotateStep::exeMove() {
    TVec3f mUpVec;
    MR::calcFrontVec(&mUpVec, this);
    MR::rotateVecDegree(&mRotateDeg, mUpVec, mRotSpeed);
    attachLift();
    MR::startLevelSound(this, "OjLvWatchTowerRotate", -1, -1, -1);
}

void WatchTowerRotateStep::exeMoveStart() {
    TVec3f mUpVec;

    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        MR::startLevelSound(this, "OjWatchTowerStart", -1, -1, -1);
    }

    MR::startLevelSound(this, "OjLvWatchTowerRotate", -1, -1, -1);

    f32 easeIn = MR::getEaseInValue(getNerveStep(), 0.0f, mRotSpeed, 180.0f);
    MR::calcFrontVec(&mUpVec, this);
    MR::rotateVecDegree(&mRotateDeg, mUpVec, easeIn);

    attachLift();

    if (MR::isStep(this, 180))
        setNerve(&WatchTowerRotateStepNrv::NrvMove::sInstance);
}

void WatchTowerRotateStep::attachLift() {
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TMatrix34<f32>* mtx = MR::getJointMtx(this, i + 1);
        PartsModel* platform = mParts[i];

        platform->mTranslation.x = mtx->val[0][3];
        platform->mTranslation.y = mtx->val[1][3];
        platform->mTranslation.z = mtx->val[2][3];
    }
}

namespace WatchTowerRotateStepNrv {
    void NrvMove::execute(Spine* pSpine) const {
        WatchTowerRotateStep* pActor = (WatchTowerRotateStep*)pSpine->mExecutor;
        pActor->exeMove();
    }

    void NrvMoveStart::execute(Spine* pSpine) const {
        WatchTowerRotateStep* pActor = (WatchTowerRotateStep*)pSpine->mExecutor;
        pActor->exeMoveStart();
    }

    void NrvWait::execute(Spine* pSpine) const {}

    NrvMove(NrvMove::sInstance);
    NrvMoveStart(NrvMoveStart::sInstance);
    NrvWait(NrvWait::sInstance);
};