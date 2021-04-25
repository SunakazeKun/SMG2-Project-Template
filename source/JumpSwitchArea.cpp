#include "spack/Actor/JumpSwitchArea.h"
#include "Player/MarioAccess.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"

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
