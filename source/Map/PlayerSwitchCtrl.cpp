#include "spack/Map/PlayerSwitchCtrl.h"
#include "Util.h"

/*
* Authors: Evanbowl, Aurum
* Objects: PlayerSwitchCtrl
* Parameters:
*  - Obj_arg0, bool, false: Check Luigi
*  - Obj_arg1, bool, false: Apply NOT logic
*  - SW_B, use, read: Needs activation first
*  - SW_A, need, write: Changes the event depending on the character
* 
* This switch controller de/activates the SW_A event when the player has selected the specified
* player character.
*/

PlayerSwitchCtrl::PlayerSwitchCtrl(const char *pName) : NameObj(pName) {
    mStageSwitchCtrl = NULL;
    mCheckLuigi = false;
    mDeactivate = false;
}

void PlayerSwitchCtrl::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);

    MR::getJMapInfoArg0NoInit(rIter, &mCheckLuigi);
    MR::getJMapInfoArg1NoInit(rIter, &mDeactivate);

    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void PlayerSwitchCtrl::movement() {
    if (mStageSwitchCtrl->isValidSwitchB() && !mStageSwitchCtrl->isOnSwitchB())
        return;

    if (MR::isPlayerLuigi() == mCheckLuigi) {
        if (mDeactivate)
            mStageSwitchCtrl->offSwitchA();
        else
            mStageSwitchCtrl->onSwitchA();
    }
}
