#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Model/ModelObj.h"
#include "Actor/Enemy/AnimScaleController.h"
#include "Actor/Enemy/WalkerStateBindStarPointer.h"

#define POIHANA_BEHAVIOR_NORMAL 0
#define POIHANA_BEHAVIOR_SLEEP 1
#define POIHANA_BEHAVIOR_NEW_HOME 2

class ActorCameraInfo;

class Poihana : public LiveActor {
public:
	Poihana(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPush(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	void exeWait();
	void exeWalkAround();
	void exeSleepStart();
	void exeSleep();
	void exeGetUp();
	void exeSearch();
	void exeChasePlayer();
	void exeShootUpCharge();
	void exeShootUp();
	void endShootUp();
	void exeGoBack();
	void exeShock();
	void exeSwoonLand();
	void exeSwoon();
	void exeRecover();
	void exeShake();
	void exeDrown();
	void exeHide();
	void exeAppear();
	void exeDPDSwoon();
	void endDPDSwoon();

	void updateBinderSensor();
	void updateTrampleBinderSensor();
	bool tryToStartBind(HitSensor*);
	void updateBindActorMtx();
	void endBind();
	void startBound();
	void calcBound();
	void contactMario(HitSensor*, HitSensor*);
	void controlVelocity();
	bool tryDrown();
	bool tryDPDSwoon();
	bool tryShock();
	bool tryHipDropShock();
	bool isNerveTypeWalkOrWait() const;
	bool isNeedForBackHome() const;
	bool isNeedForGetUp() const;
	bool isBackAttack(HitSensor*) const;

	AnimScaleController* mAnimScaleCtrl;
	WalkerStateBindStarPointer* mBindStarPointer;
	ActorCameraInfo* mCamInfo;
	LiveActor* mBindedActor;
	f32 mLaunchIntensity;
	TVec3f mRespawnPos;
	TVec3f mHomePos;
	TVec3f mFrontVec;
	s32 mBoundTimer;
	s32 mRandDir;
	s32 mBehavior;
	bool mCanDrown;
	ModelObj* mWaterColumn;
};

namespace NrvPoihana {
	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvWalkAround : public Nerve {
	public:
		NrvWalkAround() {}
		virtual void execute(Spine*) const;

		static NrvWalkAround sInstance;
	};

	class NrvSleepStart : public Nerve {
	public:
		NrvSleepStart() {}
		virtual void execute(Spine*) const;

		static NrvSleepStart sInstance;
	};

	class NrvSleep : public Nerve {
	public:
		NrvSleep() {}
		virtual void execute(Spine*) const;

		static NrvSleep sInstance;
	};

	class NrvGetUp : public Nerve {
	public:
		NrvGetUp() {}
		virtual void execute(Spine*) const;

		static NrvGetUp sInstance;
	};

	class NrvSearch : public Nerve {
	public:
		NrvSearch() {}
		virtual void execute(Spine*) const;

		static NrvSearch sInstance;
	};

	class NrvChasePlayer : public Nerve {
	public:
		NrvChasePlayer() {}
		virtual void execute(Spine*) const;

		static NrvChasePlayer sInstance;
	};

	class NrvShootUpCharge : public Nerve {
	public:
		NrvShootUpCharge() {}
		virtual void execute(Spine*) const;

		static NrvShootUpCharge sInstance;
	};

	class NrvShootUp : public Nerve {
	public:
		NrvShootUp() {}
		virtual void execute(Spine*) const;
		virtual void executeOnEnd(Spine*) const;

		static NrvShootUp sInstance;
	};

	class NrvGoBack : public Nerve {
	public:
		NrvGoBack() {}
		virtual void execute(Spine*) const;

		static NrvGoBack sInstance;
	};

	class NrvShock : public Nerve {
	public:
		NrvShock() {}
		virtual void execute(Spine*) const;

		static NrvShock sInstance;
	};

	class NrvSwoon : public Nerve {
	public:
		NrvSwoon() {}
		virtual void execute(Spine*) const;

		static NrvSwoon sInstance;
	};

	class NrvSwoonLand : public Nerve {
	public:
		NrvSwoonLand() {}
		virtual void execute(Spine*) const;

		static NrvSwoonLand sInstance;
	};

	class NrvRecover : public Nerve {
	public:
		NrvRecover() {}
		virtual void execute(Spine*) const;

		static NrvRecover sInstance;
	};

	class NrvShake : public Nerve {
	public:
		NrvShake() {}
		virtual void execute(Spine*) const;

		static NrvShake sInstance;
	};

	class NrvDrown : public Nerve {
	public:
		NrvDrown() {}
		virtual void execute(Spine*) const;

		static NrvDrown sInstance;
	};

	class NrvHide : public Nerve {
	public:
		NrvHide() {}
		virtual void execute(Spine*) const;

		static NrvHide sInstance;
	};

	class NrvAppear : public Nerve {
	public:
		NrvAppear() {}
		virtual void execute(Spine*) const;

		static NrvAppear sInstance;
	};

	class NrvDPDSwoon : public Nerve {
	public:
		NrvDPDSwoon() {}
		virtual void execute(Spine*) const;
		virtual void executeOnEnd(Spine*) const;

		static NrvDPDSwoon sInstance;
	};
};