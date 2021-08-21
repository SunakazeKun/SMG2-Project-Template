#pragma once

#include "LiveActor.h"
#include "MapObj/MapObjConnector.h"

class HitWallTimerSwitch : public LiveActor {
public:
	HitWallTimerSwitch(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void initAfterPlacement();
	virtual void calcAnim();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void exeOff();
	void exeSwitchDown();
	void exeOn();
	void exeSwitchUp();

	CollisionParts* mMoveCollision;
	MapObjConnector* mMapObjConnector;
	s32 mTimer;
	bool mActivate;
	s32 mScreenEffectType;
};

namespace NrvHitWallTimerSwitch {
	NERVE(NrvOff);
	NERVE(NrvSwitchDown);
	NERVE(NrvOn);
	NERVE(NrvSwitchUp);
};
