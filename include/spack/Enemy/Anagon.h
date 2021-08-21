#pragma once

#include "LiveActor.h"
#include "Enemy/StateController.h"

class Anagon : public LiveActor {
public:
	Anagon(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void makeActorAppeared();
	virtual void kill();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

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
	NERVE(NrvSwim);
	ENDABLE_NERVE(NrvBind);
	NERVE(NrvBreak);
	NERVE(NrvDead);
};
