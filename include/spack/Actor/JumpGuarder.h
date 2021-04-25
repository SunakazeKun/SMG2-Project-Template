#pragma once

#include "Actor/Enemy/JumpEmitter.h"

class BegomanBaby;

class JumpGuarder : public JumpEmitter {
public:
	JumpGuarder(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void control();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);

	void exeHide();
	void exeUp();
	void exeWait();
	void exeDown();
	void exeHopStart();
	void exeHopWait();
	void exeHopJump();
	void exeHopEnd();
	void exePreOpen();
	void exeOpen();
	void exeClose();
	void exeInter();
	bool enableAttack();
	bool isHit(const LiveActor*) const;
	void appearBabyFromGuarder(BegomanBaby*);

	BegomanBaby** mBabys;
	s32 mNumBabys;
	s32 mStaggerDelay;
	BegomanBaby* mPendingBabys[4];
	s32 mNumPendingBabys;
	f32 mLaunchVelocity;
};

namespace NrvJumpGuarder {
	class NrvHide : public Nerve {
	public:
		NrvHide() {}
		virtual void execute(Spine*) const;

		static NrvHide sInstance;
	};

	class NrvUp : public Nerve {
	public:
		NrvUp() {}
		virtual void execute(Spine*) const;

		static NrvUp sInstance;
	};

	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvDown : public Nerve {
	public:
		NrvDown() {}
		virtual void execute(Spine*) const;

		static NrvDown sInstance;
	};

	class NrvHopStart : public Nerve {
	public:
		NrvHopStart() {}
		virtual void execute(Spine*) const;

		static NrvHopStart sInstance;
	};

	class NrvHopWait : public Nerve {
	public:
		NrvHopWait() {}
		virtual void execute(Spine*) const;

		static NrvHopWait sInstance;
	};

	class NrvHopJump : public Nerve {
	public:
		NrvHopJump() {}
		virtual void execute(Spine*) const;

		static NrvHopJump sInstance;
	};

	class NrvHopEnd : public Nerve {
	public:
		NrvHopEnd() {}
		virtual void execute(Spine*) const;

		static NrvHopEnd sInstance;
	};

	class NrvPreOpen : public Nerve {
	public:
		NrvPreOpen() {}
		virtual void execute(Spine*) const;

		static NrvPreOpen sInstance;
	};

	class NrvOpen : public Nerve {
	public:
		NrvOpen() {}
		virtual void execute(Spine*) const;

		static NrvOpen sInstance;
	};

	class NrvClose : public Nerve {
	public:
		NrvClose() {}
		virtual void execute(Spine*) const;

		static NrvClose sInstance;
	};

	class NrvInter : public Nerve {
	public:
		NrvInter() {}
		virtual void execute(Spine*) const;

		static NrvInter sInstance;
	};
}
