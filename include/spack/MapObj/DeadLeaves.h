#pragma once

#include "MapObj/MapObjActor.h"

class DeadLeaves : public MapObjActor {
public:
	DeadLeaves(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void exeSpin();

	s32 mItemType;
};

namespace NrvDeadLeaves {
	NERVE(NrvWait);
	NERVE(NrvSpin);
};
