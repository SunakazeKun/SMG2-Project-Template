#pragma once

#include "MapObj/ChipBase.h"
#include "MapObj/ChipGroup.h"

#define SCENEOBJ_BLUE_CHIP_HOLDER 0x6E

class BlueChip : public ChipBase {
public:
	BlueChip(const char*);
};

class BlueChipGroup : public ChipGroup {
public:
	BlueChipGroup(const char*);
};
