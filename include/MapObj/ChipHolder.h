#pragma once

#include "MapObj/ChipGroup.h"

class ChipHolder : public NameObj {
public:
	ChipHolder(const char*, s32);

	virtual void init(const JMapInfoIter&);

	void registerChipGroup(ChipGroup*);
	ChipGroup* findChipGroup(s32) const;

	s32 _14;
	s32 _18;
	s32 _1C;
	s32 mType;
};

namespace MR {
	ChipHolder* createChipHolder(u32);
	ChipHolder* getChipHolder(u32);
};
