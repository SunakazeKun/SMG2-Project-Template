#pragma once

#include "Actor/NameObj/NameObj.h"

struct ActorEntry
{
    const char* pActorName; // _0
    NameObj* (*mCreationFunc)(void); // _4
};

class NameObjFactory
{
public:

    static void* getCreator(const char *);

    static ActorEntry cCreateTable[0x3EB];

    // custom functions
    static ActorEntry getEntry(s32 idx)
    {
        return cCreateTable[idx];
    }
};