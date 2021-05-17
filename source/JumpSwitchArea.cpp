#include "spack/Actor/JumpSwitchArea.h"
#include "Player/MarioAccess.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"

/*
* Created by Someone with help by Aurum
* 
* Activates its SW_A event whenever the player jumps. This can be used to replicate the Flip-Swap
* panels from SM3DL and SM3DW, for example.
*/
JumpSwitchArea::JumpSwitchArea(const char* pName) : AreaObj(pName) {
    mActivated = false;
}

void JumpSwitchArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void JumpSwitchArea::movement() {
    if (!mActivated && isInVolume(*MR::getPlayerPos()) && MR::isPlayerJumpRising() && !MR::isPlayerSwingAction()) {
        if (isOnSwitchA())
            offSwitchA();
        else
            onSwitchA();

        mActivated = true;
    }

    if (MR::isOnGroundPlayer())
        mActivated = false;
}

const char* JumpSwitchArea::getManagerName() const {
    return "SwitchArea";
}
