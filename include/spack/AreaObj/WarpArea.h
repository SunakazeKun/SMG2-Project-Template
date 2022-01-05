#pragma once

#include "AreaObj.h"
#include "spack/Extensions/WarpAreaStageTable.h"

class WarpArea : public AreaObj {
public:
    WarpArea(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void exeWarp();
    virtual const char* getManagerName() const;
    
    s32 mElapsed;
    s32 WarpID;
    s32 mode;
    bool warp;
    TVec3f pos;
    WarpAreaStageTable* StageTable;
};
