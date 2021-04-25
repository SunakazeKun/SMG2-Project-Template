#pragma once

#include "Actor/LiveActor/LiveActor.h"

class BlackHole : public LiveActor
{
public:
    BlackHole(const char *);

    virtual void makeActorAppeared();

    u8 _90[0x110-0x90];
};