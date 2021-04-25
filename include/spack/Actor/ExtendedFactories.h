#pragma once

#include "Actor/AreaObj/AreaObj.h"
#include "Actor/NameObj/NameObjFactory.h"
#include "MapObj/ChipHolder.h"
#include "MapObj/MapObjFactory.h"
#include "MapObj/MorphItemCollection.h"
#include "MapObj/PowerStar.h"
#include "MapObj/SuperSpinDriver.h"
#include "Scene/SceneObjHolder.h"
#include "spack/Actor/BallBeamer.h"
#include "spack/Actor/BlueChip.h"
#include "spack/Actor/CollectCounter.h"
#include "spack/Actor/CrystalSwitch.h"
#include "spack/Actor/DeadLeaves.h"
#include "spack/Actor/GoldenTurtle.h"
#include "spack/Actor/Jiraira.h"
#include "spack/Actor/JumpGuarder.h"
#include "spack/Actor/JumpSwitchArea.h"
#include "spack/Actor/LavaBallRisingPlanetLava.h"
#include "spack/Actor/NewMorphItemNeo.h"
#include "spack/Actor/PlayerSwitchCtrl.h"
#include "spack/Actor/PowerStarSpawner.h"
#include "spack/Actor/SensorDetector.h"
#include "spack/Actor/SwingRope.h"
#include "spack/Actor/UnizoLauncher.h"

#include "os.h"

#define NUM_ACTORS 29

struct ActorExtEntry {
    const char* pActorName;
    NameObj* (*mCreationFunc)(const char*);
};

namespace ExtendedActorFactory {
    template<typename T>
    NameObj* createExtActor(const char*);

    template<>
    NameObj* createExtActor<JumpSwitchArea>(const char* pName) {
        return new JumpSwitchArea(pName);
    }

    NameObj* createQuakeEffectArea(const char* pName) {
        if (!MR::isExistSceneObj(SCENEOBJ_QUAKE_EFFECT_GENERATOR))
            MR::createSceneObj(SCENEOBJ_QUAKE_EFFECT_GENERATOR);
        return new AreaObj(pName);
    }

    template<>
    NameObj* createExtActor<BlueChip>(const char* pName) {
        return new BlueChip(pName);
    }

    template<>
    NameObj* createExtActor<BlueChipGroup>(const char* pName) {
        return new BlueChipGroup(pName);
    }

    template<>
    NameObj* createExtActor<GoldenTurtle>(const char* pName) {
        return new GoldenTurtle(pName);
    }

    template<>
    NameObj* createExtActor<MorphItemNeoFoo>(const char* pName) {
        return new MorphItemNeoFoo("飛行オブジェ");
    }

    template<>
    NameObj* createExtActor<MorphItemNeoIce>(const char* pName) {
        return new MorphItemNeoIce("アイスオブジェ");
    }

    template<>
    NameObj* createExtActor<BallBeamer>(const char* pName) {
        return new BallBeamer(pName);
    }

    template<>
    NameObj* createExtActor<JumpGuarder>(const char* pName) {
        return new JumpGuarder(pName);
    }

    template<>
    NameObj* createExtActor<Jiraira>(const char* pName) {
        return new Jiraira(pName);
    }

    template<>
    NameObj* createExtActor<UnizoLauncher>(const char* pName) {
        return new UnizoLauncher(pName);
    }

    template<>
    NameObj* createExtActor<CollectCounter>(const char* pName) {
        return new CollectCounter(pName);
    }

    template<>
    NameObj* createExtActor<PlayerSwitchCtrl>(const char* pName) {
        return new PlayerSwitchCtrl(pName);
    }

    template<>
    NameObj* createExtActor<PowerStarSpawner>(const char* pName) {
        return new PowerStarSpawner("PowerStarSpawner");
    }

    template<>
    NameObj* createExtActor<SensorDetector>(const char* pName) {
        return new SensorDetector(pName);
    }

    template<>
    NameObj* createExtActor<CrystalSwitch>(const char* pName) {
        return new CrystalSwitch(pName);
    }

    template<>
    NameObj* createExtActor<DeadLeaves>(const char* pName) {
        return new DeadLeaves(pName);
    }

    template<>
    NameObj* createExtActor<SuperSpinDriver>(const char* pName) {
        return new SuperSpinDriver(pName, SUPER_SPIN_DRIVER_GREEN);
    }

    template<>
    NameObj* createExtActor<SwingRope>(const char* pName) {
        return new SwingRope(pName);
    }

    const ActorExtEntry cCreateTable[NUM_ACTORS] = {
        // Areas
        { "ExtraWallCheckCylinder", (NameObj* (*)(const char*))0x8033B6D0 },
        { "ForbidJumpArea", (NameObj* (*)(const char*))0x8033B6D0 },
        { "ForbidWaterSearchArea", (NameObj* (*)(const char*))0x8033B6D0 },
        { "PipeModeArea", (NameObj* (*)(const char*))0x8033B6D0 },
        { "PlaneCircularModeArea", (NameObj* (*)(const char*))0x8033B6D0 },
        { "QuakeEffectArea", createQuakeEffectArea },
        { "JumpSwitchArea", createExtActor<JumpSwitchArea> },
        // Collectibles
        { "BlueChip", createExtActor<BlueChip> },
        { "BlueChipGroup", createExtActor<BlueChipGroup> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "MorphItemCollectionFoo", (NameObj* (*)(const char*))0x8033F9B0 },
        { "MorphItemCollectionIce", (NameObj* (*)(const char*))0x8033F9B0 },
        { "MorphItemNeoFoo", createExtActor<MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<MorphItemNeoIce> },
        // Enemies
        { "BallBeamer", createExtActor<BallBeamer> },
        { "JumpGuarder", createExtActor<JumpGuarder> },
        { "Jiraira", createExtActor<Jiraira> },
        { "ShellfishBlueChip", (NameObj* (*)(const char*))0x80340710 },
        { "ShellfishPurpleCoin", (NameObj* (*)(const char*))0x80340710 },
        { "UnizoLauncher", createExtActor<UnizoLauncher> },
        // Controllers
        { "CollectCounter", createExtActor<CollectCounter> },
        { "PlayerSwitchCtrl", createExtActor<PlayerSwitchCtrl> },
        { "PowerStarSpawner", createExtActor<PowerStarSpawner> },
        { "SensorDetector", createExtActor<SensorDetector> },
        // Obstacles
        { "CrystalSwitch", createExtActor<CrystalSwitch> },
        { "DeadLeaves", createExtActor<DeadLeaves> },
        { "SuperSpinDriverGreen", createExtActor<SuperSpinDriver> },
        { "SwingRope", createExtActor<SwingRope> },
        { "UFOBlueStarCupsule", (NameObj * (*)(const char*))0x8033EC70 }
    };
};

#define NUM_CLASSES 16

namespace ExtendedMapObjFactory {
    template<typename T>
    NameObj* createExtClass(const char*);

    template<>
    NameObj* createExtClass<LavaBallRisingPlanetLava>(const char* pName) {
        return new LavaBallRisingPlanetLava(pName);
    }

    const MapObjCreatorEntry cCreateTable[NUM_CLASSES] = {
        { "AirFar100m", (NameObj* (*)(const char*))0x80341C10 },
        { "AssemblyBlock", (NameObj* (*)(const char*))0x803419D0 },
        { "BallOpener", (NameObj* (*)(const char*))0x8033E980 },
        { "BeeJumpBall", (NameObj* (*)(const char*))0x8033EA40 },
        { "CollapseRailMoveObj", (NameObj* (*)(const char*))0x803416D0 },
        { "GoroRockLaneParts", (NameObj * (*)(const char*))0x80341320 },
        { "GravityFallBlock", (NameObj* (*)(const char*))0x8033F2B0 },
        { "InvisiblePolygonObj", (NameObj* (*)(const char*))0x80342670 },
        { "JumpHole", (NameObj* (*)(const char*))0x8033F3B0 },
        { "ManholeCover", (NameObj* (*)(const char*))0x80340BB0 },
        { "RandomEffectObj", (NameObj* (*)(const char*))0x80342570 },
        { "RotateSeesawMoveObj", (NameObj* (*)(const char*))0x80341710 },
        { "ScaleMapObj", createExtClass<LavaBallRisingPlanetLava>},
        { "SimpleEnvironmentObj", (NameObj* (*)(const char*))0x80340130 },
        { "SoundSyncBlock", (NameObj* (*)(const char*))0x80341600 },
        { "WoodBox", (NameObj* (*)(const char*))0x803400B0 }
    };
};

#define NUM_SCENEOBJS 1

struct SceneObjExtEntry {
    s32 mSlotId;
    NameObj* (*mCreationFunc)(void);
};

namespace ExtendedSceneObjFactory {
    NameObj* createBlueChipHolder() {
        return new ChipHolder("ブルーチップホルダー", CHIP_BLUE);
    }

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
    const SceneObjExtEntry cCreateTable[NUM_SCENEOBJS] = {
        { 0x6E, createBlueChipHolder }
    };
};
