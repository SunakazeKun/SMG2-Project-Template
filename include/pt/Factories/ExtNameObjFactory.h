#pragma once

#include "pt/AreaObj/JumpSwitchArea.h"
#include "pt/Enemy/Anagon.h"
#include "pt/Enemy/BallBeamer.h"
#include "pt/Enemy/CocoSambo.h"
#include "pt/Enemy/DharmaSambo.h"
#include "pt/Enemy/JumpGuarder.h"
#include "pt/Enemy/Mogu.h"
#include "pt/Enemy/Poihana.h"
#include "pt/Extras/ExtQuakeEffectArea.h"
#include "pt/Extras/ExtSuperSpinDriverGreen.h"
#include "pt/Map/CollectSwitchCtrl.h"
#include "pt/Map/PlayerSwitchCtrl.h"
#include "pt/Map/SensorDetector.h"
#include "pt/MapObj/Banekiti.h"
#include "pt/MapObj/BlueChip.h"
#include "pt/MapObj/CrystalSwitch.h"
#include "pt/MapObj/DeadLeaves.h"
#include "pt/MapObj/FirePressureRadiate.h"
#include "pt/MapObj/HitWallTimerSwitch.h"
#include "pt/MapObj/Jiraira.h"
#include "pt/MapObj/NewMorphItemNeo.h"
#include "pt/MapObj/PomponPlant.h"
#include "pt/MapObj/PowerStarSpawner.h"
#include "pt/MapObj/ScrewSwitchBase.h"
#include "pt/MapObj/WatchTowerRotateStep.h"
#include "pt/MapObj/WaterLeakPipe.h"
#include "sample/GstRecord.h"
#include "sample/PadRecord.h"

class BlueStarCupsulePlanet;

namespace {
    template<typename T>
    NameObj* createExtActor(const char *pName) {
        return new T(pName);
    }

    template<>
    NameObj* createExtActor<pt::MorphItemNeoFoo>(const char *pName);

    template<>
    NameObj* createExtActor<pt::MorphItemNeoIce>(const char *pName);

    template<>
    NameObj* createExtActor<pt::PowerStarSpawner>(const char *pName);

    // ----------------------------------------------------------------------------------------------------------------

    const CreateActorEntry cNewCreateNameObjTable[] = {
        // AreaObj
        { "ExtraWallCheckCylinder", NameObjFactory::createBaseOriginCylinder<AreaObj> },
        { "ForbidJumpArea", NameObjFactory::createNameObj<AreaObj> },
        { "ForbidWaterSearchArea", NameObjFactory::createNameObj<AreaObj> },
        { "JumpSwitchArea", createExtActor<pt::JumpSwitchArea> },
        { "PipeModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        { "PlaneCircularModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        { "QuakeEffectArea", pt::createQuakeEffectArea },

        // Enemy
        { "Anagon", createExtActor<pt::Anagon> },
        { "BallBeamer", createExtActor<pt::BallBeamer> },
        { "CocoSambo", createExtActor<pt::CocoSambo> },
        { "DharmaSambo", createExtActor<pt::DharmaSambo> },
        { "JumpGuarder", createExtActor<pt::JumpGuarder> },
        { "Mogu", createExtActor<pt::Mogu> },
        { "Poihana", createExtActor<pt::Poihana> },

        // Map
        { "CollectSwitchCtrl", createExtActor<pt::CollectSwitchCtrl> },
        { "GhostLuigiRecordHelper", createExtActor<gst::GhostLuigiRecordHelper> },
        { "PadRecordHelper", createExtActor<pad::PadRecordHelper> },
        { "PlayerSwitchCtrl", createExtActor<pt::PlayerSwitchCtrl> },
        { "SensorDetector", createExtActor<pt::SensorDetector> },

        // MapObj
        { "Banekiti", createExtActor<pt::Banekiti> },
        { "BlueChip", createExtActor<pt::BlueChip> },
        { "BlueChipGroup", createExtActor<pt::BlueChipGroup> },
        { "CrystalSwitch", createExtActor<pt::CrystalSwitch> },
        { "DeadLeaves", createExtActor<pt::DeadLeaves> },
        { "FirePressureRadiate", createExtActor<pt::FirePressureRadiate> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "HitWallTimerSwitch", createExtActor<pt::HitWallTimerSwitch> },
        { "Jiraira", createExtActor<pt::Jiraira> },
        { "MorphItemCollectionFoo", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemCollectionIce", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemNeoFoo", createExtActor<pt::MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<pt::MorphItemNeoIce> },
        { "MultipleChoiceDice", createExtActor<MultipleChoiceDice> },
        { "Pompon2Plant", createExtActor<pt::PomponPlant> },
        { "PowerStarSpawner", createExtActor<pt::PowerStarSpawner> },
        { "ScrewSwitch", createExtActor<pt::ScrewSwitch> },
        { "ScrewSwitchReverse", createExtActor<pt::ScrewSwitchReverse> },
        { "ShellfishBlueChip", NameObjFactory::createNameObj<Shellfish> },
        { "ShellfishPurpleCoin", NameObjFactory::createNameObj<Shellfish> },
        { "SuperSpinDriverGreen", pt::createSuperSpinDriverGreen },
        { "UFOBlueStarCupsule", NameObjFactory::createNameObj<BlueStarCupsulePlanet> },
        { "ValveSwitch", createExtActor<pt::ValveSwitch> },
        { "WatchTowerRotateStep", createExtActor<pt::WatchTowerRotateStep>},
        { "WaterLeakPipe", createExtActor<pt::WaterLeakPipe>},

        // Ride
        { "SwingRope", createExtActor<SwingRope> }
    };

    const s32 cNewCreateNameObjTableCount = sizeof(cNewCreateNameObjTable) / sizeof(CreateActorEntry);
};
