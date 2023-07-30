#include "pt/Factories/ExtNameObjFactory.h"

/*
* Author: Aurum
*
* Extended NameObjFactory
*/

namespace {
	template<>
	NameObj* createExtActor<pt::MorphItemNeoFoo>(const char *pName) {
		return new pt::MorphItemNeoFoo("飛行オブジェ");
	}

	template<>
	NameObj* createExtActor<pt::MorphItemNeoIce>(const char *pName) {
		return new pt::MorphItemNeoIce("アイスオブジェ");
	}

	template<>
	NameObj* createExtActor<pt::PowerStarSpawner>(const char *pName) {
		return new pt::PowerStarSpawner("PowerStarSpawner");
	}

	void* getNameObjCreator(const char *pName) {
		void* creator = NameObjFactory::getCreator(pName);

		if (!creator) {
			for (s32 i = 0; i < cNewCreateNameObjTableCount; i++) {
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
}
