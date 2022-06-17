#pragma once

#include "syati.h"

namespace pt {
	class CollectSwitchCtrl : public NameObj {
	public:
		CollectSwitchCtrl(const char *pName);

		virtual void init(const JMapInfoIter &rIter);
		virtual void movement();

		StageSwitchCtrl* mStageSwitchCtrl;
		s32 mItem;
		s32 mCount;
		s32 mComparator;
		bool mSwitchOnce;
	};
};
