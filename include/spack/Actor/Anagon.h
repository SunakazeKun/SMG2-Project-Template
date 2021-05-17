#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Enemy/AnimScaleController.h"
#include "Actor/Enemy/WalkerStateBindStarPointer.h"
#include "JGeometry/TMatrix34.h"

class Anagon : public LiveActor {
public:
	Anagon(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void makeActorAppeared();
	virtual void kill();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

	void exeSwim();
	void exeBind();
	void endBind();
	void exeBreak();
	void exeDead();

	bool isStarPointerPointing();

	LiveActor* mStarPointerTargets[3];
	AnimScaleController* mAnimScaleCtrl;
};

static const Vec sStarPointerTargetOffset[4] = {
	{ 0.0f, 0.0f, 150.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }
};

static const float sStarPointerTargetSize[4] = {
	220.0f, 150.0f, 120.0f, 120.0f
};

static const char* sStarPointerTargetJoint[4] = {
	"Joint00", "Joint01", "Joint02", "Joint03"
};

namespace NrvAnagon {
	class NrvSwim : public Nerve {
	public:
		NrvSwim() {}
		virtual void execute(Spine*) const;

		static NrvSwim sInstance;
	};

	class NrvBind : public Nerve {
	public:
		NrvBind() {}
		virtual void execute(Spine*) const;
		virtual void executeOnEnd(Spine*) const;

		static NrvBind sInstance;
	};

	class NrvBreak : public Nerve {
	public:
		NrvBreak() {}
		virtual void execute(Spine*) const;

		static NrvBreak sInstance;
	};

	class NrvDead : public Nerve {
	public:
		NrvDead() {}
		virtual void execute(Spine*) const;

		static NrvDead sInstance;
	};
};
