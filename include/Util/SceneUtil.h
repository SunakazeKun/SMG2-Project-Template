#pragma once

#include "JMap/JMapInfoIter.h"

class NameObj;
namespace MR {
    void incCoin(int);

    s32 getCoinNum();
    s32 getPurpleCoinNum();
    s32 getPowerStarNum();

    s32 getChildObjNum(const JMapInfoIter&);
    void getChildObjName(const char**, const JMapInfoIter&, int);
    void initChildObj(NameObj*, const JMapInfoIter&, int);

    NameObj* initChildObj(const JMapInfoIter&, int);

    const char* getCurrentStageName();
};
