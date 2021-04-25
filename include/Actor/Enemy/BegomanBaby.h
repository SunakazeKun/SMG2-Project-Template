#pragma once

#include "Actor/Enemy/BegomanBase.h"

class BegomanBaby : public BegomanBase {
public:
	BegomanBaby(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void appear();
	virtual void kill();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPush(u32, HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveMsgOther(u32, HitSensor*, HitSensor*);
	virtual void funcBegomanBase1();
	virtual void funcBegomanBase2();
	virtual s32 getKind() const;
	virtual bool onTouchElectric(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	virtual void setNerveReturn();
	virtual void setNerveWait();

	void appearFromGuarder();

	s32 _10C;
	f32 _110[12];
	LiveActor* mHost;
	u8 _144;
};

namespace NrvBegomanBaby {
	class HostTypeNrvLaunchFromGuarder : public Nerve {
	public:
		HostTypeNrvLaunchFromGuarder() {}
		virtual void execute(Spine*) const;
		static HostTypeNrvLaunchFromGuarder sInstance;
	};
};
