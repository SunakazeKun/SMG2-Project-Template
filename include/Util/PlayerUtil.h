#pragma once

#include "JGeometry/TVec3.h"

class LiveActor;

namespace MR
{
    JGeometry::TVec3<f32>* getPlayerPos();

    JGeometry::TVec3<f32>* getPlayerVelocity();

    bool isPlayerElementModeTornado();
    bool isPlayerElementModeInvincible();
    bool isPlayerElementModeBee();
    bool isPlayerElementModeHopper();
    bool isPlayerElementModeTeresa();
    bool isPlayerElementModeIce();
    bool isPlayerElementModeFire();
    bool isPlayerElementModeRock();
    bool isPlayerElementModeCloud();
    bool isPlayerElementModeNormal();

    bool isOnGroundPlayer();
    bool isPlayerStaggering();
    bool isPlayerSwimming();
    bool isPlayerSleeping();
    bool isPlayerJumpRising();
    bool isPlayerSwingAction();

    bool isPlayerHipDropLand();

    bool isPlayerInBind();

    void endBindAndPlayerJump(LiveActor *, const JGeometry::TVec3<f32> &, u32);

    void startBckPlayer(const char *, const char *);

    void setPlayerSwingPermission(bool);

    void setPlayerJumpVec(const JGeometry::TVec3<f32>&);
    void forceJumpPlayer(const JGeometry::TVec3<f32>&);
};