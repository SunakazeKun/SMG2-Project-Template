#pragma once

#include "kamek.h"
#include "Actor/NameObj/NameObj.h"
#include "JMap/JMapInfoIter.h"

class StageSwitchCtrl {
public:
    StageSwitchCtrl(const JMapInfoIter &);

    void onSwitchA();
    void offSwitchA();
    bool isOnSwitchA() const;
    bool isValidSwitchA() const;
    void onSwitchB();
    void offSwitchB();
    bool isOnSwitchB() const;
    bool isValidSwitchB() const;
    bool isOnSwitchAppear() const;
    bool isValidSwitchAppear() const;
    void onSwitchDead();
    void offSwitchDead();
    bool isValidSwitchDead() const;
    bool isOnAllSwitchAfterB(int) const;
    bool isOnAnyOneSwitchAfterB(int) const;

    u8 _0[0x10];
};

namespace MR {
    StageSwitchCtrl* createStageSwitchCtrl(NameObj *, const JMapInfoIter &);
};
