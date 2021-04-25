#include "spack/Actor/PlayerSwitchCtrl.h"
#include "Util/EventUtil.h"
#include "Util/JMapUtil.h"
#include "Util/ObjUtil.h"

PlayerSwitchCtrl::PlayerSwitchCtrl(const char* pName) : NameObj(pName) {
    mStageSwitchCtrl = NULL;
    mCheckLuigi = false;
    mDeactivate = false;
}

void PlayerSwitchCtrl::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mCheckLuigi);
    MR::getJMapInfoArg1NoInit(rIter, &mDeactivate);
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void PlayerSwitchCtrl::movement() {
    if ((!mStageSwitchCtrl->isValidSwitchB() || mStageSwitchCtrl->isOnSwitchB()) && MR::isPlayerLuigi() == mCheckLuigi) {
        if (mDeactivate)
            mStageSwitchCtrl->offSwitchA();
        else
            mStageSwitchCtrl->onSwitchA();
    }
}
