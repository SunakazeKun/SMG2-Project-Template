#include "spack/Extensions/ObjFactories.h"
#include "MapObj/ChipHolder.h"
#include "Scene/SceneObjHolder.h"
#include "Util.h"

/*
* Authors: shibbo, Aurum
*/
namespace SPack {
    /*
    * This is shibbo's ExtendedActorFactory code that allows us to add new objects to the game by
    * implementing a separate objects table. Thus, we do not have to touch NameObjFactory's table.
    */
    void* getNameObjCreator(const char *pName) {
        void* creator = NameObjFactory::getCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_ACTORS; i++) {
                const CreateActorEntry entry = SPack::cNewCreateNameObjTable[i];

                if (MR::isEqualStringCase(entry.pActorName, pName)) {
                    creator = entry.mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x803394C0, getNameObjCreator); // redirection hook
    kmCall(0x803430C8, getNameObjCreator);
    kmCall(0x804564D4, getNameObjCreator);

    /*
    * Luckily, SMG2 added ProductMapObjDataTable to easily define many basic objects such as skies,
    * platforms, effects and more. However, a lot of classes are not compatible with this. For the
    * sake of customizability, a bunch of actor classes were added, including AssemblyBlock and the
    * newly added ScaleMapObj.
    */
    void* getMapObjCreator(ProductMapCreator *pFactory, const char *pName) {
        void* creator = pFactory->getObjClassName(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_CLASSES; i++) {
                const CreateActorEntry entry = SPack::cNewCreateMapObjTable[i];

                if (MR::isEqualStringCase(entry.pActorName, pName)) {
                    creator = entry.mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x8026305C, getMapObjCreator); // redirection hook

    /*
    * A few slots for custom SceneObj instances are unused, but accessing them can be a bit tricky.
    * New instances are necessary for a couple of objects, such as the blue chips. These use their
    * own ChipHolder instance to manage the different groups and collection counter layout.
    */
    NameObj* createSceneObj(SceneObjHolder *pHolder, s32 type) {
        NameObj* sceneObj = pHolder->newEachObj(type);

        if (!sceneObj) {
            for (s32 i = 0; i < NUM_SCENEOBJS; i++) {
                const CreateSceneObjEntry* e = &SPack::cNewCreateSceneObjTable[i];

                if (e->mSlotId == type) {
                    sceneObj = e->mCreationFunc();
                    break;
                }
            }
        }

        return sceneObj;
    }

    kmCall(0x804599D0, createSceneObj); // redirection hook
}
