#pragma once

#include "Actor/LiveActor/LiveActor.h"

class PowerStar : public LiveActor {
public:
	PowerStar(const char* pName);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void appear();
	virtual void makeActorAppeared();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	s32 mPowerStarID;
	s32 _94;
	JGeometry::TVec3<f32> _98;
	JGeometry::TVec3<f32> _A4;
	JGeometry::TVec3<f32> _B0;
	f32 _BC[12];
	f32 _EC[12];
	s32 _11C;
	s32 _120;
	s32 _124;
	s32 _128;
	bool _12C;
	bool _12D;
	s32 mColor;
	bool mIsGrandStar;
	bool mIsGreenStar;
	bool _136;
	bool _137;
	bool _138;
	s32 _140;
	f32 _144[12];
	bool _174;
};
