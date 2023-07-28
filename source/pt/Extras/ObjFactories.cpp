#include "pt/Extras/ObjFactories.h"

/*
* Authors: shibbo, Aurum
*/
namespace pt {
    /*
    * Extended NameObjFactory
    */

    template<>
    NameObj* createExtActor<MorphItemNeoFoo>(const char *pName) {
        return new MorphItemNeoFoo("飛行オブジェ");
    }

    template<>
    NameObj* createExtActor<MorphItemNeoIce>(const char *pName) {
        return new MorphItemNeoIce("アイスオブジェ");
    }

    template<>
    NameObj* createExtActor<PowerStarSpawner>(const char *pName) {
        return new PowerStarSpawner("PowerStarSpawner");
    }

    NameObj* createQuakeEffectArea(const char *pName) {
        if (!MR::isExistSceneObj(SCENE_OBJ_QUAKE_EFFECT_GENERATOR)) {
            MR::createSceneObj(SCENE_OBJ_QUAKE_EFFECT_GENERATOR);
        }

        return new AreaObj(pName);
    }

    NameObj* createSuperSpinDriverGreen(const char *pName) {
        return new SuperSpinDriver(pName, SUPER_SPIN_DRIVER_GREEN);
    }

    void* getNameObjCreator(const char *pName) {
        void* creator = NameObjFactory::getCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_ACTORS; i++) {
                const CreateActorEntry entry = cNewCreateNameObjTable[i];

                if (MR::isEqualString(entry.pActorName, pName)) {
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
    * Extended ProjectMapCreator
    */
    void* getMapObjCreator(ProductMapCreator *pFactory, const char *pName) {
        void* creator = pFactory->getProductMapCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_CLASSES; i++) {
                const CreateActorEntry entry = cNewCreateMapObjTable[i];

                if (MR::isEqualString(entry.pActorName, pName)) {
                    creator = entry.mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x8026305C, getMapObjCreator); // redirection hook

    /*
    * Extended SceneObjHolder
    */
    NameObj* createSceneObj(SceneObjHolder *pHolder, s32 type) {
        NameObj* sceneObj = pHolder->newEachObj(type);

        if (!sceneObj) {
            for (s32 i = 0; i < NUM_SCENEOBJS; i++) {
                const CreateSceneObjEntry e = cNewCreateSceneObjTable[i];

                if (e.mSlotId == type) {
                    sceneObj = e.mCreationFunc();
                    break;
                }
            }
        }

        return sceneObj;
    }

    kmCall(0x804599D0, createSceneObj); // redirection hook
}
