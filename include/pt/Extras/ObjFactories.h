#pragma once

#include "pt/AreaObj/JumpSwitchArea.h"
#include "pt/Enemy/Anagon.h"
#include "pt/Enemy/BallBeamer.h"
#include "pt/Enemy/CocoSambo.h"
#include "pt/Enemy/DharmaSambo.h"
#include "pt/Enemy/JumpGuarder.h"
#include "pt/Enemy/Mogu.h"
#include "pt/Enemy/Poihana.h"
#include "pt/Map/CollectSwitchCtrl.h"
#include "pt/Map/PlayerSwitchCtrl.h"
#include "pt/Map/SensorDetector.h"
#include "pt/MapObj/Banekiti.h"
#include "pt/MapObj/BlueChip.h"
#include "pt/MapObj/ChooChooTrain.h"
#include "pt/MapObj/CrystalSwitch.h"
#include "pt/MapObj/DeadLeaves.h"
#include "pt/MapObj/FirePressureRadiate.h"
#include "pt/MapObj/HitWallTimerSwitch.h"
#include "pt/MapObj/Jiraira.h"
#include "pt/MapObj/LavaBallRisingPlanetLava.h"
#include "pt/MapObj/NewMorphItemNeo.h"
#include "pt/MapObj/PomponPlant.h"
#include "pt/MapObj/PowerStarSpawner.h"
#include "pt/MapObj/ScrewSwitchBase.h"
#include "pt/MapObj/WatchTowerRotateStep.h"
#include "pt/MapObj/WaterLeakPipe.h"

/*
* SMG2 actor class definitions for use with "NameObjFactory::createNameObj". We only need these declarations. The actual
* templated functions already exist so the linker will do the remaining work for us.
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
class Shellfish;
class SimpleEnvironmentObj;
class SimpleTextureSwitchChangeObj;
class WoodBox;

namespace pt {
    template<typename T>
    NameObj* createExtActor(const char *pName) {
        return new T(pName);
    }

    struct CreateSceneObjEntry {
        s32 mSlotId;
        NameObj* (*mCreationFunc)(void);
    };


    /*
    * Extended NameObjFactory
    */

    template<>
    NameObj* createExtActor<MorphItemNeoFoo>(const char *pName);

    template<>
    NameObj* createExtActor<MorphItemNeoIce>(const char *pName);

    template<>
    NameObj* createExtActor<PowerStarSpawner>(const char *pName);

    NameObj* createQuakeEffectArea(const char *pName);
    NameObj* createSuperSpinDriverGreen(const char *pName);

    /*
    * Number of total new actor entries. This value has to adjusted everytime a creation function
    * is added or removed!
    */
    #define NUM_ACTORS 43

    const CreateActorEntry cNewCreateNameObjTable[NUM_ACTORS] = {
        // AreaObj
        { "ExtraWallCheckCylinder", NameObjFactory::createBaseOriginCylinder<AreaObj> },
        { "ForbidJumpArea", NameObjFactory::createNameObj<AreaObj> },
        { "ForbidWaterSearchArea", NameObjFactory::createNameObj<AreaObj> },
        { "JumpSwitchArea", createExtActor<JumpSwitchArea> },
        { "PipeModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        { "PlaneCircularModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        { "QuakeEffectArea", createQuakeEffectArea },
        // Enemy
        { "Anagon", createExtActor<Anagon> },
        { "BallBeamer", createExtActor<BallBeamer> },
        { "CocoSambo", createExtActor<CocoSambo> },
        { "DharmaSambo", createExtActor<DharmaSambo> },
        { "JumpGuarder", createExtActor<JumpGuarder> },
        { "Mogu", createExtActor<Mogu> },
        { "Poihana", createExtActor<Poihana> },
        // Map
        { "CollectSwitchCtrl", createExtActor<CollectSwitchCtrl> },
        { "PlayerSwitchCtrl", createExtActor<PlayerSwitchCtrl> },
        { "SensorDetector", createExtActor<SensorDetector> },
        // MapObj
        { "Banekiti", createExtActor<Banekiti> },
        { "BlueChip", createExtActor<BlueChip> },
        { "BlueChipGroup", createExtActor<BlueChipGroup> },
        { "CrystalSwitch", createExtActor<CrystalSwitch> },
        { "DeadLeaves", createExtActor<DeadLeaves> },
        { "FirePressureRadiate", createExtActor<FirePressureRadiate> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "HitWallTimerSwitch", createExtActor<HitWallTimerSwitch> },
        { "Jiraira", createExtActor<Jiraira> },
        { "MorphItemCollectionFoo", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemCollectionIce", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemNeoFoo", createExtActor<MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<MorphItemNeoIce> },
        { "MultipleChoiceDice", createExtActor<MultipleChoiceDice> },
        { "Pompon2Plant", createExtActor<PomponPlant> },
        { "PowerStarSpawner", createExtActor<PowerStarSpawner> },
        { "ScrewSwitch", createExtActor<ScrewSwitch> },
        { "ScrewSwitchReverse", createExtActor<ScrewSwitchReverse> },
        { "ShellfishBlueChip", NameObjFactory::createNameObj<Shellfish> },
        { "ShellfishPurpleCoin", NameObjFactory::createNameObj<Shellfish> },
        { "SuperSpinDriverGreen", createSuperSpinDriverGreen },
        { "UFOBlueStarCupsule", NameObjFactory::createNameObj<BlueStarCupsulePlanet> },
        { "ValveSwitch", createExtActor<ValveSwitch> },
        { "WatchTowerRotateStep", createExtActor<WatchTowerRotateStep>},
        { "WaterLeakPipe", createExtActor<WaterLeakPipe>},
        // Ride
        { "SwingRope", createExtActor<SwingRope> }
    };


    /*
    * Extended ProductMapCreator
    */

    /*
    * Number of total new class entries. This value has to be adjusted everytime a creation function
    * is added or removed!
    */
    #define NUM_CLASSES 16

    const CreateActorEntry cNewCreateMapObjTable[NUM_CLASSES] = {
        { "AirFar100m", NameObjFactory::createNameObj<AirFar100m> },
        { "AssemblyBlock", NameObjFactory::createNameObj<AssemblyBlock> },
        { "BallOpener", NameObjFactory::createNameObj<BallOpener> },
        { "BeeJumpBall", NameObjFactory::createNameObj<BeeJumpBall> },
        { "ChooChooTrain", createExtActor<ChooChooTrain> },
        { "CollapseRailMoveObj", NameObjFactory::createNameObj<CollapseRailMoveObj> },
        { "GoroRockLaneParts", NameObjFactory::createNameObj<GoroRockLaneParts> },
        { "GravityFallBlock", NameObjFactory::createNameObj<GravityFallBlock> },
        { "InvisiblePolygonObj", NameObjFactory::createNameObj<InvisiblePolygonObj> },
        { "JumpHole", NameObjFactory::createNameObj<JumpHole> },
        { "ManholeCover", NameObjFactory::createNameObj<ManholeCover> },
        { "ScaleMapObj", createExtActor<LavaBallRisingPlanetLava>},
        { "SimpleEnvironmentObj", NameObjFactory::createNameObj<SimpleEnvironmentObj> },
        { "SimpleTextureSwitchChangeObj", NameObjFactory::createNameObj<SimpleTextureSwitchChangeObj> },
        { "RepeatTimerSwitchingBlock", NameObjFactory::createNameObj<RepeatTimerBlock> },
        { "WoodBox", NameObjFactory::createNameObj<WoodBox> }
    };


    /*
    * Extended SceneObjHolder
    */

    /*
    * Number of total new SceneObj instances. This value has to adjusted everytime a creation func-
    * tion is added or removed!
    */
    #define NUM_SCENEOBJS 1

    /*
    * There is a bunch of free SceneObj slots that we can still use as desired. The following slots
    * are unused as of now: 0x3E, 0x47, 0x48, 0x49, 0x54, 0x5A, 0x68, 0x69 and 0x7B.
    */
    const CreateSceneObjEntry cNewCreateSceneObjTable[NUM_SCENEOBJS] = {
        { SCENE_OBJ_BLUE_CHIP_HOLDER, createBlueChipHolder }
    };
};
