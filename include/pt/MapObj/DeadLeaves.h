#pragma once

#include "syati.h"

namespace pt {
	class DeadLeaves : public LiveActor {
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
};
