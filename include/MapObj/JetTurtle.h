#pragma once

#include "Actor/LiveActor/LiveActor.h"

class JetTurtle : public LiveActor
{
public:
	JetTurtle(const char*);

	virtual void init(JMapInfoIter const&);
	virtual void initAfterPlacement();
	virtual void appear();
	virtual void control();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);
	virtual void exeThrowing();
	virtual void resetPosition();
	virtual void reset(u32);

	u32 _90;
	u32 _94;
	u32 _98;
	u32 _9C;
	u32 _A0;
	u32 _A4;
	u32 _A8;
	u32 _AC;
	u32 _B0;
	u32 _B4;
	u32 _B8;
	u32 _BC;
	u32 _C0;
	u32 _C4;
	u32 _C8;
	u32 _CC;
	u32 _D0;
	u32 _D4;
	u32 _D8;
	u32 _DC;
	u32 _E0;
	u32 _E4;
	u32 _E8;
	u32 _EC;
};