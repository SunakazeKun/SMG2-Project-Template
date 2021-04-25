#pragma once

#include "Actor/NameObj/NameObj.h"
#include "Actor/Switch/StageSwitch.h"

class PlayerSwitchCtrl : public NameObj {
public:
    PlayerSwitchCtrl(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    StageSwitchCtrl* mStageSwitchCtrl;
    bool mCheckLuigi;
    bool mDeactivate;
};
