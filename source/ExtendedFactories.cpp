#include "spack/Actor/ExtendedFactories.h"
#include "MapObj/ChipHolder.h"
#include "Scene/SceneObjHolder.h"
#include "Util/StringUtil.h"

namespace SPack {
    /*
    * This is a modified version of shibbs' ExtendedActorFactory code, however, that one
    * did not pass the actor's name to the creation function. This makes adding different
    * objects of the same class a bit difficult. It was a simple fix, though.
    */
    void* getNameObjCreator(const char* pName) {
        void* creator = NameObjFactory::getCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_ACTORS; i++) {
                const ActorExtEntry* e = &ExtendedActorFactory::cCreateTable[i];

                if (MR::isEqualStringCase(e->pActorName, pName)) {
                    creator = e->mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x804564D4, getNameObjCreator); // redirection hook
    kmCall(0x803430C8, getNameObjCreator);

    /*
    * Luckily, SMG2 added ProductMapObjDataTable to easily add many basic objects such
    * as platforms, skies, effects and more. However, a lot of classes are not supported
    * by this. In order to allow better customizability, a bunch of classes were added,
    * including AssemblyBlock and the new ScaleMapObj.
    */
    void* getMapObjCreator(MapObjFactory* pFactory, const char* pName) {
        void* creator = pFactory->getCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < NUM_CLASSES; i++) {
                const MapObjCreatorEntry* e = &ExtendedMapObjFactory::cCreateTable[i];

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
    * A few slots for SceneObj instances are still free to use, but accessing them can
    * be quite difficult. This is necessary for some object's like blue chips which use
    * a special ChipHolder instance to control the collection counter.
    */
    NameObj* createSceneObj(SceneObjHolder* pHolder, s32 type) {
        NameObj* sceneObj = pHolder->newEachObj(type);

        if (!sceneObj) {
            for (s32 i = 0; i < NUM_SCENEOBJS; i++) {
                const SceneObjExtEntry* e = &ExtendedSceneObjFactory::cCreateTable[i];

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
