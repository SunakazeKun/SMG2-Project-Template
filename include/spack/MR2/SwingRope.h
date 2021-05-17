#pragma once

#include "Actor/LiveActor/LiveActor.h"

class ActorCameraInfo;
class SledRopePoint;
class SwingRopePoint;

class SwingRope : public LiveActor {
public:
	SwingRope(const char *);

	virtual void init(const JMapInfoIter &);
	virtual void draw() const;
	virtual void updateHitSensor(HitSensor *);
	virtual void attackSensor(HitSensor *, HitSensor *);
	virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

	TVec3f _8C;
	f32 _98;
	u8 _9C;
	s32 _A0;
	s32 _A4;
	s32 _A8;
	SwingRopePoint** _AC;
	LiveActor* _B0;
	Mtx _B4;
	f32 _E4;
	f32 _E8;
	f32 _EC;
	SledRopePoint* _F0;
	ActorCameraInfo* _F4;
	f32 _F8;
	f32 _FC;
	f32 _100;
	TVec3f _104;
	TVec3f _110;
	f32 _11C;
	s32 _120;
};
