#include "pt/Factories/ExtProjectMapCreator.h"

namespace {
    /*
    * Extended ProjectMapCreator
    */
    void* getMapObjCreator(ProductMapCreator *pFactory, const char *pName) {
        CreateActorFunc* creator = pFactory->getProductMapCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < cNewCreateMapObjTableCount; i++) {
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
}
