#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Enemy/RingBeam.h"
#include "Util/Functor.h"

class BallBeamer : public LiveActor {
public:
	BallBeamer(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

	void syncSwitchOnA();
	void syncSwitchOffA();
	void syncSwitchOnB();
	void setupAttack();
	void exeWait();
	void exeAttack();
	void exeInter();

	RingBeam** mBeams;
	s32 mNumMaxBeams;
	s32 mAttackDuration;
	f32 mActiveRange;
	bool mActive;
	Mtx mHeadMtx;
};

namespace NrvBallBeamer {
	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvAttack : public Nerve {
	public:
		NrvAttack() {}
		virtual void execute(Spine*) const;

		static NrvAttack sInstance;
	};

	class NrvInter : public Nerve {
	public:
		NrvInter() {}
		virtual void execute(Spine*) const;

		static NrvInter sInstance;
	};
};
