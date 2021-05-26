#pragma once

#include "Actor/Model/ModelObj.h"
#include "JGeometry/TQuat4.h"

class MoguStone : public ModelObj {
public:
	MoguStone(const char*, const char*);

	virtual void init(const JMapInfoIter&);
	virtual void appear();
	virtual void kill();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
	virtual void doBehavior();
	virtual void startBreakSound();
	virtual void startThrowLevelSound();

	void exeThrow();
	void exeFall();
	void emit(bool, const TVec3f&, const TVec3f&, f32);
	bool isTaken();

	TQuat4f mUpQuat;
	TQuat4f _A0;
	TVec3f mFrontVec;
	f32 mSpeed;
	bool mCalcGravity;
};

namespace NrvMoguStone {
	class NrvTaken : public Nerve {
	public:
		NrvTaken() {}

		virtual void execute(Spine *) const;

		static NrvTaken sInstance;
	};

	class NrvThrow : public Nerve {
	public:
		NrvThrow() {}

		virtual void execute(Spine *) const;

		static NrvThrow sInstance;
	};

	class NrvFall : public Nerve {
	public:
		NrvFall() {}

		virtual void execute(Spine *) const;

		static NrvFall sInstance;
	};
};
