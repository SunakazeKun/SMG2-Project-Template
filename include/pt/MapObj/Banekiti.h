#pragma once

#include "syati.h"

namespace pt {
	class Banekiti : public LiveActor {
	public:
		Banekiti(const char *pName);

		virtual void init(const JMapInfoIter &rIter);
		virtual void control();
		virtual void calcAndSetBaseMtx();
		virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
		virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

		void exeWait();
		void exeRepel();
		void exeDPDSwoon();
		void endDPDSwoon();

		AnimScaleController* mAnimScaleCtrl;
		WalkerStateBindStarPointer* mBindStarPointer;
		MapPartsRailMover* mRailMover;
	};

	namespace NrvBanekiti {
		NERVE(NrvWait);
		NERVE(NrvRepel);
		ENDABLE_NERVE(NrvDPDSwoon);
	};
};
