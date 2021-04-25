#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "JGeometry/TVec3.h"
#include "MapObj/ChipBase.h"

#define CHIP_BLUE 0
#define CHIP_YELLOW 1

class ChipBase : public LiveActor {
public:
	ChipBase(const char*, s32, const char*);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void makeActorAppeared();
	virtual void makeActorDead();
	virtual void control();
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	void initModel(const JMapInfoIter&);
	void initSensor();
	void initShadow(const JMapInfoIter&);
	void initJMapParam(const JMapInfoIter&);
	void deactive();
	void setGroupID(s32);
	void setHost(LiveActor*);
	void appearWait();
	void appearFlashing();
	void requestGet(HitSensor*, HitSensor*);
	void requestShow();
	void requestHide();
	void requestStartControl();
	void requestEndControl();
	void exeFlashing();
	void exeGot();
	// there is a bit more

	s32 _90;
	s32 _94;
	s32 _98;
	s32 _9C;
	s32 _A0;
	f32 _A4;
	f32 _A8;
	f32 _AC;
	s32 mArg0;
	s32 _B4;
	s32 _B8;
	u8 _BC;
	bool mArg2;
};
