#pragma once

#include "MapObj/ChipBase.h"
#include "MapObj/ChipGroup.h"
#include "MapObj/ChipHolder.h"

class BlueChip : public ChipBase {
public:
	BlueChip(const char *pName);
};

class BlueChipGroup : public ChipGroup {
public:
	BlueChipGroup(const char *pName);
};

namespace SPack {
	NameObj* createBlueChipHolder();
	ChipHolder* getBlueChipHolder();
};
