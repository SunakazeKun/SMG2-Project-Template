#pragma once

#include "pt/MapObj/ChooChooTrain.h"
#include "pt/MapObj/LavaBallRisingPlanetLava.h"

/*
* SMG2 actor class definitions for use with "NameObjFactory::createNameObj". We only need these declarations. The actual
* templated functions already exist so the linker will do the remaining work for us.
*/
class AirFar100m;
class AssemblyBlock;
class BallOpener;
class BeeJumpBall;
class GoroRockLaneParts;
class GravityFallBlock;
class InvisiblePolygonObj;
class JumpHole;
class ManholeCover;
class RailMoveTimerObj;
class RepeatTimerSwitchingBlock;
class Shellfish;
class SimpleEnvironmentObj;
class SimpleTextureSwitchChangeObj;
class WoodBox;

namespace {
    template<typename T>
    NameObj* createExtActor(const char *pName) {
        return new T(pName);
    }

    const CreateActorEntry cNewCreateMapObjTable[] = {
        { "AirFar100m", NameObjFactory::createNameObj<AirFar100m> },
        { "AssemblyBlock", NameObjFactory::createNameObj<AssemblyBlock> },
        { "BallOpener", NameObjFactory::createNameObj<BallOpener> },
        { "BeeJumpBall", NameObjFactory::createNameObj<BeeJumpBall> },
        { "ChooChooTrain", createExtActor<pt::ChooChooTrain> },
        { "GoroRockLaneParts", NameObjFactory::createNameObj<GoroRockLaneParts> },
        { "GravityFallBlock", NameObjFactory::createNameObj<GravityFallBlock> },
        { "InvisiblePolygonObj", NameObjFactory::createNameObj<InvisiblePolygonObj> },
        { "JumpHole", NameObjFactory::createNameObj<JumpHole> },
        { "ManholeCover", NameObjFactory::createNameObj<ManholeCover> },
        { "RailMoveTimerObj", NameObjFactory::createNameObj<RailMoveTimerObj> },
        { "RepeatTimerSwitchingBlock", NameObjFactory::createNameObj<RepeatTimerSwitchingBlock> },
        { "ScaleMapObj", createExtActor<pt::LavaBallRisingPlanetLava> },
        { "SimpleEnvironmentObj", NameObjFactory::createNameObj<SimpleEnvironmentObj> },
        { "SimpleTextureSwitchChangeObj", NameObjFactory::createNameObj<SimpleTextureSwitchChangeObj> },
        { "WoodBox", NameObjFactory::createNameObj<WoodBox> }
    };

    const s32 cNewCreateMapObjTableCount = sizeof(cNewCreateMapObjTable) / sizeof(CreateActorEntry);
};
