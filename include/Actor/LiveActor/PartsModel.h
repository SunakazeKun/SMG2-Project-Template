#pragma once

#include "Actor/LiveActor/LiveActor.h"

class PartsModel : public LiveActor
{
public:
    PartsModel(LiveActor *, const char *, const char *, Mtx, s32, bool);

    u32 _90;
    u32 _94;
    u32 _98;
    u8 _9C;
    u8 _9D;
    u8 _9E;
};