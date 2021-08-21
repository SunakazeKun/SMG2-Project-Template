#pragma once

#include "NameObj.h"
#include "Map/Switch/StageSwitch.h"

class PlayerSwitchCtrl : public NameObj {
public:
    PlayerSwitchCtrl(const char *pName);

    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();

    StageSwitchCtrl* mStageSwitchCtrl;
    bool mCheckLuigi;
    bool mDeactivate;
};
