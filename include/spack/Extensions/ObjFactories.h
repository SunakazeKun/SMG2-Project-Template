#pragma once

#include "NameObj/NameObjFactory.h"
#include "Map/ProductMapCreator.h"
#include "MapObj/ChipHolder.h"
#include "MapObj/ItemDice.h"
#include "MapObj/JetTurtle.h"
#include "MapObj/MorphItemCollection.h"
#include "MapObj/SuperSpinDriver.h"
#include "Ride/SwingRope.h"
#include "Scene/SceneObjHolder.h"
#include "spack/AreaObj/JumpSwitchArea.h"
#include "spack/AreaObj/WarpArea.h"
#include "spack/Enemy/Anagon.h"
#include "spack/Enemy/BallBeamer.h"
#include "spack/Enemy/Banekiti.h"
#include "spack/Enemy/CocoSambo.h"
#include "spack/Enemy/JumpGuarder.h"
#include "spack/Enemy/Mogu.h"
#include "spack/Enemy/Poihana.h"
#include "spack/Map/CollectSwitchCtrl.h"
#include "spack/Map/PlayerSwitchCtrl.h"
#include "spack/Map/SensorDetector.h"
#include "spack/MapObj/BlueChip.h"
#include "spack/MapObj/CrystalSwitch.h"
#include "spack/MapObj/DeadLeaves.h"
#include "spack/MapObj/FirePressureRadiate.h"
#include "spack/MapObj/HitWallTimerSwitch.h"
#include "spack/MapObj/Jiraira.h"
#include "spack/MapObj/LavaBallRisingPlanetLava.h"
#include "spack/MapObj/NewMorphItemNeo.h"
#include "spack/MapObj/PomponPlant.h"
#include "spack/MapObj/PowerStarSpawner.h"
#include "spack/MapObj/ScrewSwitchBase.h"
#include "spack/MapObj/SwitchBox.h"
#include "spack/MapObj/WatchTowerRotateStep.h"
#include "spack/MapObj/WaterLeakPipe.h"
/*
* Alias that allows static addresses to be used as creation functions. DEPRECATED since 2021-08-21!
* This is only kept here for backwards compatibility, but it will be removed in future versions.
*/
typedef NameObj* (*ExternCreator)(const char *pName);

struct CreateSceneObjEntry {
    s32 mSlotId;
    NameObj* (*mCreationFunc)(void);
};

/*
* Macro for defining a simple creation function for an actor. The class name and actor name have to
* be specified. If you don't want to specify a name, just use "pName" instead.
*/
#define CREATE_EXT_ACTOR(name, actorName)\
template<>\
NameObj* createExtActor<name>(const char *pName) {\
    return new name(actorName);\
}\

/*
* SMG2 actor class definitions for use with "NameObjFactory::createNameObj". We only need these
* declarations for said function's parameter in order to call their creation functions. The actual
* template function instances already exist so the linker will do the remaining work for us.
*/
class AirFar100m;
class AssemblyBlock;
class BallOpener;
class BeeJumpBall;
class BlueStarCupsulePlanet;
class CollapseRailMoveObj;
class GoroRockLaneParts;
class GravityFallBlock;
class InvisiblePolygonObj;
class JumpHole;
class ManholeCover;
class RepeatTimerBlock;
class RotateSeesawStep;
class Shellfish;
class SimpleEnvironmentObj;
class WoodBox;

namespace SPack {
    template<typename T>
    NameObj* createExtActor(const char *pName);

    /*********************************************************************************************/
    /* 1 - Extended NameObjFactory entries                                                       */
    /*********************************************************************************************/
    CREATE_EXT_ACTOR(JumpSwitchArea, pName);
    CREATE_EXT_ACTOR(Anagon, pName);
    CREATE_EXT_ACTOR(BallBeamer, pName);
    CREATE_EXT_ACTOR(Banekiti, pName);
    CREATE_EXT_ACTOR(CocoSambo, pName);
    CREATE_EXT_ACTOR(JumpGuarder, pName);
    CREATE_EXT_ACTOR(Mogu, pName);
    CREATE_EXT_ACTOR(Poihana, pName);
    CREATE_EXT_ACTOR(CollectSwitchCtrl, pName);
    CREATE_EXT_ACTOR(PlayerSwitchCtrl, pName);
    CREATE_EXT_ACTOR(SensorDetector, pName);
    CREATE_EXT_ACTOR(BlueChip, pName);
    CREATE_EXT_ACTOR(BlueChipGroup, pName);
    CREATE_EXT_ACTOR(GoldenTurtle, pName);
    CREATE_EXT_ACTOR(MorphItemNeoFoo, "飛行オブジェ");
    CREATE_EXT_ACTOR(MorphItemNeoIce, "アイスオブジェ");
    CREATE_EXT_ACTOR(PowerStarSpawner, "PowerStarSpawner");
    CREATE_EXT_ACTOR(SwitchBox, pName);
    CREATE_EXT_ACTOR(SwitchDice, pName);
    CREATE_EXT_ACTOR(FirePressureRadiate, pName);
    CREATE_EXT_ACTOR(Jiraira, pName);
    CREATE_EXT_ACTOR(CrystalSwitch, pName);
    CREATE_EXT_ACTOR(DeadLeaves, pName);
    CREATE_EXT_ACTOR(HitWallTimerSwitch, pName);
    CREATE_EXT_ACTOR(PomponPlant, pName);
    CREATE_EXT_ACTOR(ScrewSwitch, pName);
    CREATE_EXT_ACTOR(ScrewSwitchReverse, pName);
    CREATE_EXT_ACTOR(ValveSwitch, pName);
    CREATE_EXT_ACTOR(WarpArea, pName);
    CREATE_EXT_ACTOR(WatchTowerRotateStep, pName);
    CREATE_EXT_ACTOR(WaterLeakPipe, pName);
    CREATE_EXT_ACTOR(SwingRope, pName);

    NameObj* createQuakeEffectArea(const char *pName) {
        if (!MR::isExistSceneObj(SceneObj_QuakeEffectGenerator))
            MR::createSceneObj(SceneObj_QuakeEffectGenerator);
        return new AreaObj(pName);
    }

    NameObj* createSuperSpinDriverGreen(const char *pName) {
        return new SuperSpinDriver(pName, SUPER_SPIN_DRIVER_GREEN);
    }

    /*
    * Number of total new actor entries. This value has to adjusted everytime a creation function
    * is added or removed!
    */
    #define NUM_ACTORS 44

    const CreateActorEntry cNewCreateNameObjTable[NUM_ACTORS] = {
        // AreaObj
        { "ExtraWallCheckCylinder", NameObjFactory::createBaseOriginCylinder<AreaObj> },
        { "ForbidJumpArea", NameObjFactory::createNameObj<AreaObj> },
        { "ForbidWaterSearchArea", NameObjFactory::createNameObj<AreaObj> },
        { "JumpSwitchArea", createExtActor<JumpSwitchArea> },
        { "PipeModeArea", NameObjFactory::createNameObj<AreaObj> },
        { "PlaneCircularModeArea", NameObjFactory::createNameObj<AreaObj> },
        { "QuakeEffectArea", createQuakeEffectArea },
        { "WarpArea", createExtActor<WarpArea> },
        // Enemy
        { "Anagon", createExtActor<Anagon> },
        { "BallBeamer", createExtActor<BallBeamer> },
        { "Banekiti", createExtActor<Banekiti> },
        { "CocoSambo", createExtActor<CocoSambo> },
        { "JumpGuarder", createExtActor<JumpGuarder> },
        { "Mogu", createExtActor<Mogu> },
        { "Poihana", createExtActor<Poihana> },
        // Map
        { "CollectSwitchCtrl", createExtActor<CollectSwitchCtrl> },
        { "PlayerSwitchCtrl", createExtActor<PlayerSwitchCtrl> },
        { "SensorDetector", createExtActor<SensorDetector> },
        // MapObj
        { "BlueChip", createExtActor<BlueChip> },
        { "BlueChipGroup", createExtActor<BlueChipGroup> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "MorphItemCollectionFoo", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemCollectionIce", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemNeoFoo", createExtActor<MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<MorphItemNeoIce> },
        { "PowerStarSpawner", createExtActor<PowerStarSpawner> },
        { "SuperSpinDriverGreen", createSuperSpinDriverGreen },
        { "SwitchDice", createExtActor<SwitchDice> },
        { "FirePressureRadiate", createExtActor<FirePressureRadiate> },
        { "Jiraira", createExtActor<Jiraira> },
        { "ShellfishBlueChip", NameObjFactory::createNameObj<Shellfish> },
        { "ShellfishPurpleCoin", NameObjFactory::createNameObj<Shellfish> },
        { "CrystalSwitch", createExtActor<CrystalSwitch> },
        { "DeadLeaves", createExtActor<DeadLeaves> },
        { "HitWallTimerSwitch", createExtActor<HitWallTimerSwitch> },
        { "Pompon2Plant", createExtActor<PomponPlant> },
        { "ScrewSwitch", createExtActor<ScrewSwitch> },
        { "ScrewSwitchReverse", createExtActor<ScrewSwitchReverse> },
        { "SwitchBox", createExtActor<SwitchBox> },
        { "ValveSwitch", createExtActor<ValveSwitch> },
        { "UFOBlueStarCupsule", NameObjFactory::createNameObj<BlueStarCupsulePlanet> },
        { "WatchTowerRotateStep", createExtActor<WatchTowerRotateStep>},
        { "WaterLeakPipe", createExtActor<WaterLeakPipe>},
        // Ride
        { "SwingRope", createExtActor<SwingRope> }
    };

    /*********************************************************************************************/
    /* 2 - Extended ProductMapObjDataTable classes                                               */
    /*********************************************************************************************/
    CREATE_EXT_ACTOR(LavaBallRisingPlanetLava, pName);

    /*
    * Number of total new class entries. This value has to adjusted everytime a creation function
    * is added or removed!
    */
    #define NUM_CLASSES 15

    const CreateActorEntry cNewCreateMapObjTable[NUM_CLASSES] = {
        { "AirFar100m", NameObjFactory::createNameObj<AirFar100m> },
        { "AssemblyBlock", NameObjFactory::createNameObj<AssemblyBlock> },
        { "BallOpener", NameObjFactory::createNameObj<BallOpener> },
        { "BeeJumpBall", NameObjFactory::createNameObj<BeeJumpBall> },
        { "CollapseRailMoveObj", NameObjFactory::createNameObj<CollapseRailMoveObj> },
        { "GoroRockLaneParts", NameObjFactory::createNameObj<GoroRockLaneParts> },
        { "GravityFallBlock", NameObjFactory::createNameObj<GravityFallBlock> },
        { "InvisiblePolygonObj", NameObjFactory::createNameObj<InvisiblePolygonObj> },
        { "JumpHole", NameObjFactory::createNameObj<JumpHole> },
        { "ManholeCover", NameObjFactory::createNameObj<ManholeCover> },
        { "RotateSeesawMoveObj", NameObjFactory::createNameObj<RotateSeesawStep> },
        { "ScaleMapObj", createExtActor<LavaBallRisingPlanetLava>},
        { "SimpleEnvironmentObj", NameObjFactory::createNameObj<SimpleEnvironmentObj> },
        { "SoundSyncBlock", NameObjFactory::createNameObj<RepeatTimerBlock> },
        { "WoodBox", NameObjFactory::createNameObj<WoodBox> }
    };

    /*********************************************************************************************/
    /* 3 - Extended SceneObjHolder instances                                                     */
    /*********************************************************************************************/

    /*
    * Number of total new SceneObj instances. This value has to adjusted everytime a creation func-
    * tion is added or removed!
    */
    #define NUM_SCENEOBJS 1

    /*
    * There is a bunch of free SceneObj slots that we can still use as desired. The following slots
    * are unused as of now: 0x16, 0x3E, 0x47, 0x48, 0x49, 0x54, 0x5A, 0x68, 0x69 and 0x7B.
    */
    const CreateSceneObjEntry cNewCreateSceneObjTable[NUM_SCENEOBJS] = {
        { 0x6E, SPack::createBlueChipHolder }
    };
};
