#include "spack/Actor/WatchTowerRotateStep.h"
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

WatchTowerRotateStep::WatchTowerRotateStep(const char* pName) : LiveActor(pName) {
    mParts = NULL;
    mRotSpeed = 300.0f;
}

void WatchTowerRotateStep::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WatchTowerRotateStep", NULL);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);

    HitSensor* senBody = MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "WatchTowerRotateStep", senBody, NULL);
    initEffectKeeper(0, NULL, false);

    initSound(4, "WatchTowerRotateStep", 0, TVec3f(0.0f, 0.0f, 0.0f));
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
    JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > mfrontUp;

    MR::calcFrontVec(&mUpVec, this);
    MR::makeMtxFrontUpPos(&mfrontUp, mUpVec, mRotateDeg, mTranslation);
    MR::setBaseTRMtx(this, mfrontUp);
}

void WatchTowerRotateStep::initLift(const JMapInfoIter& rIter) {
    mParts = new PartsModel*[4];

    for (s32 i = 0; i < 4; i++) {
        JGeometry::TMatrix34<f32>* mtx = MR::getJointMtx(this, i + 1);
        PartsModel* mdl = new PartsModel(this, "物見の塔リフト", "WatchTowerRotateStepLift", mtx->val, -1, 0);
        mParts[i] = mdl;
        MR::initCollisionParts(mdl, "WatchTowerRotateStepLift", getSensor("Body"), NULL);

        mdl->initShadowControllerList(1);
        TVec3f shadowOffs(600.0f, 200.0f, 400.0f);
        MR::addShadowVolumeBox(mdl, "Body", shadowOffs, *mdl->getBaseMtx());

        MR::setShadowVolumeStartDropOffset(mdl, "WatchTowerRotateStepLift", 300.0f);
        MR::setShadowDropLength(mdl, "WatchTowerRotateStepLift", 370.0f);
        mdl->_9C = 0;

        if (MR::isDemoCast(this, NULL))
            MR::tryRegisterDemoCast(mdl, rIter);

        mdl->initWithoutIter();
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
        PartsModel* curMdl = mParts[i];

        curMdl->mTranslation.x = mtx->val[0][3];
        curMdl->mTranslation.y = mtx->val[1][3];
        curMdl->mTranslation.z = mtx->val[2][3];
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