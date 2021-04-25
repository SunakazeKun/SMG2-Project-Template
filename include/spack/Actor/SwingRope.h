#pragma once

#include "Actor/LiveActor/LiveActor.h"

class SwingRope : public LiveActor {
public:
	SwingRope(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void draw() const;
	virtual void updateHitSensor(HitSensor*);
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	f32 _8C;
	f32 _90;
	f32 _94;
	f32 _98;
	u8 _9C;
	s32 _A0;
	s32 _A4;
	s32 _A8;
	s32 _AC;
	s32 _B0;
	f32 _B4[12];
	f32 _E4;
	f32 _E8;
	f32 _EC;
	s32 _F0;
	s32 _F4;
	f32 _F8;
	f32 _FC;
	f32 _100;
	f32 _104;
	f32 _108;
	f32 _10C;
	f32 _110;
	f32 _114;
	f32 _118;
	f32 _11C;
	s32 _120;
};
