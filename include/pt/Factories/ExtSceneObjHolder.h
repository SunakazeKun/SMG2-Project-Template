#pragma once

#include "pt/MapObj/BlueChip.h"

struct CreateSceneObjEntry {
    s32 mSlotId;
    NameObj* (*mCreationFunc)(void);
};

namespace {
    /*
    * There is a bunch of free SceneObj slots that we can still use as desired. The following slots
    * are unused as of now: 0x3E, 0x47, 0x48, 0x49, 0x54, 0x5A, 0x68, 0x69 and 0x7B.
    */
    const CreateSceneObjEntry cNewCreateSceneObjTable[] = {
        { SCENE_OBJ_BLUE_CHIP_HOLDER, pt::createBlueChipHolder }
    };

    const s32 cNewCreateSceneObjTableCount = sizeof(cNewCreateSceneObjTable) / sizeof(CreateActorEntry);
};
