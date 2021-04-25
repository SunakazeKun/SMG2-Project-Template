#pragma once

#include "kamek.h"

class LiveActor;

namespace MR
{
    void startSound(const LiveActor *, const char *, s32, s32);
    void startLevelSound(const LiveActor *, const char *, s32, s32, s32);
};