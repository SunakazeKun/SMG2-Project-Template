#pragma once

#include "Actor/AreaObj/AreaObj.h"
#include "Actor/NameObj/NameObjFactory.h"
#include "Actor/NameObj/PlanetCreator.h"
#include "MapObj/ChipHolder.h"
#include "MapObj/JetTurtle.h"
#include "MapObj/MorphItemCollection.h"
#include "MapObj/SuperSpinDriver.h"
#include "Scene/SceneObjHolder.h"
#include "spack/Actor/Anagon.h"
#include "spack/Actor/BallBeamer.h"
#include "spack/Actor/BlueChip.h"
#include "spack/Actor/CollectSwitchCtrl.h"
#include "spack/Actor/CrystalSwitch.h"
#include "spack/Actor/DeadLeaves.h"
#include "spack/Actor/HitWallTimerSwitch.h"
#include "spack/Actor/Jiraira.h"
#include "spack/Actor/JumpGuarder.h"
#include "spack/Actor/JumpSwitchArea.h"
#include "spack/Actor/LavaBallRisingPlanetLava.h"
#include "spack/Actor/NewMorphItemNeo.h"
#include "spack/Actor/PlayerSwitchCtrl.h"
#include "spack/Actor/Poihana.h"
#include "spack/Actor/PomponPlant.h"
#include "spack/Actor/PowerStarSpawner.h"
#include "spack/Actor/SensorDetector.h"
#include "spack/Actor/ScrewSwitchBase.h"
#include "spack/Actor/UnizoLauncher.h"
#include "spack/Actor/WatchTowerRotateStep.h"
#include "spack/Actor/WaterLeakPipe.h"
#include "spack/MR2/SwingRope.h"

#define NUM_ACTORS 38
#define NUM_CLASSES 16
#define NUM_SCENEOBJS 1

typedef NameObj* (*ExternCreator)(const char *);

struct CreateActorEntry {
    const char* pActorName;
    NameObj* (*mCreationFunc)(const char *);
};

struct CreateSceneObjEntry {
    s32 mSlotId;
    NameObj* (*mCreationFunc)(void);
};

namespace SPack {
    template<typename T>
    NameObj* createExtActor(const char *);

    // NameObjFactory extensions

    template<>
    NameObj* createExtActor<JumpSwitchArea>(const char * pName) {
        return new JumpSwitchArea(pName);
    }

    NameObj* createQuakeEffectArea(const char * pName) {
        if (!MR::isExistSceneObj(102))
            MR::createSceneObj(102);
        return new AreaObj(pName);
    }

    template<>
    NameObj* createExtActor<BlueChip>(const char * pName) {
        return new BlueChip(pName);
    }

    template<>
    NameObj* createExtActor<BlueChipGroup>(const char * pName) {
        return new BlueChipGroup(pName);
    }

    template<>
    NameObj* createExtActor<GoldenTurtle>(const char * pName) {
        return new GoldenTurtle(pName);
    }

    template<>
    NameObj* createExtActor<MorphItemNeoFoo>(const char * pName) {
        return new MorphItemNeoFoo("飛行オブジェ");
    }

    template<>
    NameObj* createExtActor<MorphItemNeoIce>(const char * pName) {
        return new MorphItemNeoIce("アイスオブジェ");
    }

    template<>
    NameObj* createExtActor<Anagon>(const char * pName) {
        return new Anagon(pName);
    }

    template<>
    NameObj* createExtActor<BallBeamer>(const char * pName) {
        return new BallBeamer(pName);
    }

    template<>
    NameObj* createExtActor<JumpGuarder>(const char * pName) {
        return new JumpGuarder(pName);
    }

    template<>
    NameObj* createExtActor<Jiraira>(const char * pName) {
        return new Jiraira(pName);
    }

    template<>
    NameObj* createExtActor<Poihana>(const char * pName) {
        return new Poihana(pName);
    }

    template<>
    NameObj* createExtActor<UnizoLauncher>(const char * pName) {
        return new UnizoLauncher(pName);
    }

    template<>
    NameObj* createExtActor<CollectSwitchCtrl>(const char * pName) {
        return new CollectSwitchCtrl(pName);
    }

    template<>
    NameObj* createExtActor<PlayerSwitchCtrl>(const char * pName) {
        return new PlayerSwitchCtrl(pName);
    }

    template<>
    NameObj* createExtActor<PowerStarSpawner>(const char * pName) {
        return new PowerStarSpawner("PowerStarSpawner");
    }

    template<>
    NameObj* createExtActor<SensorDetector>(const char * pName) {
        return new SensorDetector(pName);
    }

    template<>
    NameObj* createExtActor<CrystalSwitch>(const char * pName) {
        return new CrystalSwitch(pName);
    }

    template<>
    NameObj* createExtActor<HitWallTimerSwitch>(const char * pName) {
        return new HitWallTimerSwitch(pName);
    }

    template<>
    NameObj* createExtActor<ScrewSwitch>(const char * pName) {
        return new ScrewSwitch(pName);
    }

    template<>
    NameObj* createExtActor<ScrewSwitchReverse>(const char * pName) {
        return new ScrewSwitchReverse(pName);
    }

    template<>
    NameObj* createExtActor<ValveSwitch>(const char * pName) {
        return new ValveSwitch(pName);
    }

    template<>
    NameObj* createExtActor<DeadLeaves>(const char * pName) {
        return new DeadLeaves(pName);
    }

    template<>
    NameObj* createExtActor<PomponPlant>(const char * pName) {
        return new PomponPlant(pName);
    }

    template<>
    NameObj* createExtActor<SuperSpinDriver>(const char * pName) {
        return new SuperSpinDriver(pName, SUPER_SPIN_DRIVER_GREEN);
    }

    template<>
    NameObj* createExtActor<SwingRope>(const char * pName) {
        return new SwingRope(pName);
    }

    template<>
    NameObj* createExtActor<WatchTowerRotateStep>(const char * pName) {
        return new WatchTowerRotateStep(pName);
    }

    template<>
    NameObj* createExtActor<WaterLeakPipe>(const char * pName) {
        return new WaterLeakPipe(pName);
    }

    const CreateActorEntry cNewCreateNameObjTable[NUM_ACTORS] = {
        // Areas
        { "ExtraWallCheckCylinder", (ExternCreator)0x8033B6D0 },
        { "ForbidJumpArea", (ExternCreator)0x8033B6D0 },
        { "ForbidWaterSearchArea", (ExternCreator)0x8033B6D0 },
        { "JumpSwitchArea", createExtActor<JumpSwitchArea> },
        { "PipeModeArea", (ExternCreator)0x8033B6D0 },
        { "PlaneCircularModeArea", (ExternCreator)0x8033B6D0 },
        { "QuakeEffectArea", createQuakeEffectArea },
        // Collectibles
        { "BlueChip", createExtActor<BlueChip> },
        { "BlueChipGroup", createExtActor<BlueChipGroup> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "MorphItemNeoFoo", createExtActor<MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<MorphItemNeoIce> },
        // Enemies
        { "Anagon", createExtActor<Anagon> },
        { "BallBeamer", createExtActor<BallBeamer> },
        { "JumpGuarder", createExtActor<JumpGuarder> },
        { "Jiraira", createExtActor<Jiraira> },
        { "Poihana", createExtActor<Poihana> },
        { "ShellfishBlueChip", (ExternCreator)0x80340710 },
        { "ShellfishPurpleCoin", (ExternCreator)0x80340710 },
        { "UnizoLauncher", createExtActor<UnizoLauncher> },
        // Controllers
        { "CollectSwitchCtrl", createExtActor<CollectSwitchCtrl> },
        { "PlayerSwitchCtrl", createExtActor<PlayerSwitchCtrl> },
        { "PowerStarSpawner", createExtActor<PowerStarSpawner> },
        { "SensorDetector", createExtActor<SensorDetector> },
        // Switches
        { "CrystalSwitch", createExtActor<CrystalSwitch> },
        { "HitWallTimerSwitch", createExtActor<HitWallTimerSwitch> },
        { "ScrewSwitch", createExtActor<ScrewSwitch> },
        { "ScrewSwitchReverse", createExtActor<ScrewSwitchReverse> },
        { "ValveSwitch", createExtActor<ValveSwitch> },
        // Decoration
        { "DeadLeaves", createExtActor<DeadLeaves> },
        { "MorphItemCollectionFoo", (ExternCreator)0x8033F9B0 },
        { "MorphItemCollectionIce", (ExternCreator)0x8033F9B0 },
        { "Pompon2Plant", createExtActor<PomponPlant> },
        // Platforms
        { "UFOBlueStarCupsule", (ExternCreator)0x8033EC70 },
        { "WatchTowerRotateStep", createExtActor<WatchTowerRotateStep>},
        { "WaterLeakPipe", createExtActor<WaterLeakPipe>},
        // Miscellaneous
        { "SuperSpinDriverGreen", createExtActor<SuperSpinDriver> },
        { "SwingRope", createExtActor<SwingRope> }
    };

    // ProductMapObjDataTable classes

    template<>
    NameObj* createExtActor<LavaBallRisingPlanetLava>(const char* pName) {
        return new LavaBallRisingPlanetLava(pName);
    }

    const CreateActorEntry cNewCreateMapObjTable[NUM_CLASSES] = {
        { "AirFar100m", (ExternCreator)0x80341C10 },
        { "AssemblyBlock", (ExternCreator)0x803419D0 },
        { "BallOpener", (ExternCreator)0x8033E980 },
        { "BeeJumpBall", (ExternCreator)0x8033EA40 },
        { "CollapseRailMoveObj", (ExternCreator)0x803416D0 },
        { "GoroRockLaneParts", (ExternCreator)0x80341320 },
        { "GravityFallBlock", (ExternCreator)0x8033F2B0 },
        { "InvisiblePolygonObj", (ExternCreator)0x80342670 },
        { "JumpHole", (ExternCreator)0x8033F3B0 },
        { "ManholeCover", (ExternCreator)0x80340BB0 },
        { "RandomEffectObj", (ExternCreator)0x80342570 },
        { "RotateSeesawMoveObj", (ExternCreator)0x80341710 },
        { "ScaleMapObj", createExtActor<LavaBallRisingPlanetLava>},
        { "SimpleEnvironmentObj", (ExternCreator)0x80340130 },
        { "SoundSyncBlock", (ExternCreator)0x80341600 },
        { "WoodBox", (ExternCreator)0x803400B0 }
    };

    NameObj* createBlueChipHolder() {
        return new ChipHolder("ブルーチップホルダー", CHIP_BLUE);
    }

    // SceneObjHolder instances

    /*
    * Some SceneObj slots are unused and we are free to use these. Free
    * slot IDs are:
    * - 0x16
    * - 0x3E
    * - 0x47
    * - 0x48
    * - 0x49
    * - 0x54
    * - 0x5A
    * - 0x68
    * - 0x69
    * - 0x6E (taken by BlueChipHolder)
    * - 0x7B
    */
    const CreateSceneObjEntry cNewCreateSceneObjTable[NUM_SCENEOBJS] = {
        { 0x6E, createBlueChipHolder }
    };
};
