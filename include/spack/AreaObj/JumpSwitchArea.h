#pragma once

#include "AreaObj.h"

class JumpSwitchArea : public AreaObj {
public:
    JumpSwitchArea(const char *pName);

    virtual void init(const JMapInfoIter &rIter);
    virtual void movement();
    virtual const char* getManagerName() const;

    bool mActivated;
};
