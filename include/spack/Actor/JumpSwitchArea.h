#pragma once

#include "Actor/AreaObj/AreaObj.h"

class JumpSwitchArea : public AreaObj {
public:
    JumpSwitchArea(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual const char* getManagerName() const;

    bool mActivated;
};
