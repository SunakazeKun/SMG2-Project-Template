#include "spack/Actor/ExtendedFactories.h"
#include "MapObj/ChipHolder.h"
#include "Scene/SceneObjHolder.h"
#include "Util/StringUtil.h"

namespace SPack {
    /*
    * This is shibbo's ExtendedActorFactory code that allows us to easily add new objects to the game
    * without having to modify the game's NameObjFactory creation table.
    */
    void* getNameObjCreator(const char* pName) {
        void* creator = NameObjFactory::getCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_ACTORS; i++) {
                const CreateActorEntry* e = &SPack::cNewCreateNameObjTable[i];

                if (MR::isEqualStringCase(e->pActorName, pName)) {
                    creator = e->mCreationFunc;
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
    * Luckily, SMG2 added ProductMapObjDataTable to easily add many basic objects such as skies,
    * platforms, effects and more. However, a lot of classes are not supported by this. In order to
    * allow better customizability, a bunch of classes were added, including AssemblyBlock and the
    * new ScaleMapObj.
    */
    void* getMapObjCreator(ProductMapCreator* pFactory, const char* pName) {
        void* creator = pFactory->getObjClassName(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_CLASSES; i++) {
                const CreateActorEntry* e = &SPack::cNewCreateMapObjTable[i];

                if (MR::isEqualStringCase(e->pActorName, pName)) {
                    creator = e->mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x8026305C, getMapObjCreator); // redirection hook

    /*
    * A few slots for new SceneObj instances are free to use, but accessing them can be a bit
    * tricky. New SceneObjs are necessary for some object's, such as blue chips, which use a
    * special ChipHolder instance to control the collection counter.
    */
    NameObj* createSceneObj(SceneObjHolder* pHolder, s32 type) {
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
