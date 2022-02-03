#pragma once 

#include "LiveActor.h"

class SwitchBox : public LiveActor {
public:
	SwitchBox(const char *);
	
	virtual void init(const JMapInfoIter& rIter);
	virtual bool receiveMessage(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void exeOn();
	void exeReturn();
	void exe2P();

	s32 mTimer;
	bool mUseRespawn;
	bool mUseTimerSe;
	bool mDisableP2;
};

namespace NrvSwitchBox {
	NERVE(NrvWait);
	NERVE(NrvBreak);
	NERVE(NrvReturn);
};
