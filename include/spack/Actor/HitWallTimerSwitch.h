#include "Actor/LiveActor/LiveActor.h"
#include "MapObj/MapObjConnector.h"

class HitWallTimerSwitch : public LiveActor {
public:
	HitWallTimerSwitch(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void calcAnim();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	void exeOff();
	void exeSwitchDown();
	void exeOn();
	void exeSwitchUp();

	CollisionParts* mMoveCollision;
	MapObjConnector* mMapObjConnector;
	s32 mTimer;
	bool mActivate;
};

namespace NrvHitWallTimerSwitch {
	class NrvOff : public Nerve {
	public:
		NrvOff() {}
		virtual void execute(Spine*) const;

		static NrvOff sInstance;
	};

	class NrvSwitchDown : public Nerve {
	public:
		NrvSwitchDown() {}
		virtual void execute(Spine*) const;

		static NrvSwitchDown sInstance;
	};

	class NrvOn : public Nerve {
	public:
		NrvOn() {}
		virtual void execute(Spine*) const;

		static NrvOn sInstance;
	};

	class NrvSwitchUp : public Nerve {
	public:
		NrvSwitchUp() {}
		virtual void execute(Spine*) const;

		static NrvSwitchUp sInstance;
	};
};
