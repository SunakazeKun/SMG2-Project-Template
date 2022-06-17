#pragma once

#include "syati.h"

namespace pt {
    class PlayerSwitchCtrl : public NameObj {
    public:
        PlayerSwitchCtrl(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void movement();

        StageSwitchCtrl* mStageSwitchCtrl;
        bool mCheckLuigi;
        bool mDeactivate;
    };
};
