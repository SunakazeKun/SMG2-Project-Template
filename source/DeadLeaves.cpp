#include "spack/Actor/DeadLeaves.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"

/*
* Created by Aurum
* 
* This is the Autumn Leaves from Gold Leaf Galaxy that can spawn items when using the Spin Attack
* in its range. Since this seemed to be a simple object to port, it is included here. It functions
* exactly like in SMG1, but it can activate SW_A after using the Spin Attack if enabled.
*/
DeadLeaves::DeadLeaves(const char* pName) : MapObjActor(pName) {
	mItemType = -1;
}

void DeadLeaves::init(const JMapInfoIter& rIter) {
	MapObjActor::init(rIter);
	MapObjActorInitInfo initInfo;
	MapObjActorUtil::setupInitInfoSimpleMapObj(&initInfo);

	initInfo.setupHitSensor();
	initInfo.setupHitSensorParam(8, 70.0f, TVec3f(0.0f, 30.0f, 0.0f));
	initInfo.setupNerve(&NrvDeadLeaves::NrvWait::sInstance);

	initialize(rIter, initInfo);

	MR::getJMapInfoArg0NoInit(rIter, &mItemType);
	MR::useStageSwitchWriteA(this, rIter);
	
	if (mItemType == 0)
		MR::declareCoin(this, 1);
	else if (mItemType == 1)
		MR::declareStarPiece(this, 3);
}

u32 DeadLeaves::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgPlayerSpinAttack(msg)) {
		setNerve(&NrvDeadLeaves::NrvSpin::sInstance);
		return 1;
	}
	return 0;
}

void DeadLeaves::exeSpin() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Spin");
		MR::startLevelSound(this, "OjLeavesSwing", -1, -1, -1);

		if (mItemType == 0)
			MR::appearCoinPop(this, mTranslation, 1);
		else if (mItemType == 1) {
			if (MR::appearStarPiece(this, mTranslation, 3, 10.0f, 40.0f, false))
				MR::startLevelSound(this, "OjStarPieceBurst", -1, -1, -1);
		}

		if (MR::isValidSwitchA(this))
			MR::onSwitchA(this);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvDeadLeaves::NrvWait::sInstance);
}

namespace NrvDeadLeaves {
	void NrvWait::execute(Spine* spine) const {}

	void NrvSpin::execute(Spine* spine) const {
		DeadLeaves* pActor = (DeadLeaves*)spine->mExecutor;
		pActor->exeSpin();
	}

	NrvWait(NrvWait::sInstance);
	NrvSpin(NrvSpin::sInstance);
}
