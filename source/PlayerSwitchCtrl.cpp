#include "spack/Actor/PlayerSwitchCtrl.h"
#include "Util/ActorInitUtil.h"
#include "Util/EventUtil.h"
#include "Util/JMapUtil.h"
#include "Util/ObjUtil.h"

/*
* Created by Evanbowl with help by Aurum
* 
* This switch controller de/activates an event when the specified player character is selected.
* If specified, the controller will start to check for the player character if its SW_B is on.
*/
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
