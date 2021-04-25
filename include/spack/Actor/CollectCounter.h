#pragma once

#include "Actor/NameObj/NameObj.h"
#include "Actor/Switch/StageSwitch.h"

class CollectCounter : public NameObj {
public:
	CollectCounter(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void movement();

	StageSwitchCtrl* mStageSwitchCtrl;
	s32 mItem;
	s32 mCount;
	s32 mComparator;
	bool mSwitchOnce;
};
