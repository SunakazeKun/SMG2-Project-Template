#include "spack/Actor/BlueChip.h"
#include "MapObj/ChipCounter.h"
#include "MapObj/ChipHolder.h"
#include "Scene/SceneObjHolder.h"

BlueChip::BlueChip(const char* pName) : ChipBase(pName, CHIP_BLUE, "BlueChip") {}

BlueChipGroup::BlueChipGroup(const char* pName) : ChipGroup(pName, CHIP_BLUE) {}

namespace SPack {
	/*
	* There is one ChipHolder for every chip type, however, the blue one has been completely
	* removed from SMG2 which leaves us with the challenge to reimplement it. Whenever a
	* chip or chip group is initialized, the game tries to create a ChipHolder if it does not
	* exist already. Interestingly, the game performs a check to see if the chip type is blue,
	* but it simply returns from the function in that case.
	*/
	void createChipHolder(s32 type) {
		if (type == CHIP_BLUE)
			MR::createSceneObj(SCENEOBJ_BLUE_CHIP_HOLDER);
		else if (type == CHIP_YELLOW)
			MR::createSceneObj(SCENEOBJ_YELLOW_CHIP_HOLDER);
	}

	kmCall(0x80282580, createChipHolder); // redirect hook in ChipBase::init(const JMapInfo&)
	kmCall(0x80284484, createChipHolder); // redirect hook in ChipGroup::init(const JMapInfo&)

	/*
	* MR::getChipHolder(u32) checks for the blue type, but never retrieves a ChipHolder for
	* blue chips as it has been removed from the game. In that case, NULL is returned instead.
	* However, we can simply insert a call to return our new blue chip holder here.
	*/
	ChipHolder* getBlueChipHolder() {
		return (ChipHolder*)MR::getSceneObjHolder()->getObj(SCENEOBJ_BLUE_CHIP_HOLDER);
	}

	kmCall(0x80284E80, getBlueChipHolder); // reinsert call

	/*
	* As expected, the function which loads the counter layouts misses the code for the blue
	* chip counter as well. We can easily add this back, but we have to copy some of the original
	* code to make it work properly. This entire thing can be heavily improved when written in
	* pure assembly but I'm not sure how to accomplish this here...
	*/
	void initChipCounter(ChipCounter* pActor) {
		char* layoutName;
		if (pActor->mType == CHIP_BLUE)
			layoutName = "BlueChipCounter";
		else if (pActor->mType == CHIP_YELLOW)
			layoutName = "YellowChipCounter";
		pActor->initLayoutManager(layoutName, 2);
	}

	kmWrite32(0x80283894, 0x7F63DB78); // "mr r3, r27" so that r3 is "this->"
	kmCall(0x80283898, initChipCounter); // redirect hook
	kmWrite32(0x8028389C, 0x48000014); // skip useless instructions
};
