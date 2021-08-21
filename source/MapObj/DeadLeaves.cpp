#include "spack/MapObj/DeadLeaves.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: DeadLeaves
* Parameters:
*  - Obj_arg0, list, -1: Item type
*   *: Nothing
*   0: One Coin
*   1: Three Star Bits
*  - SW_A, use, write: Activated when spinning
*  - SW_AWAKE
* 
* A pile of autumn leaves from Gold Leaf Galaxy. When using the Spin Attack in its range, it can
* spawn an item and activate a SW_A event as well.
*/

DeadLeaves::DeadLeaves(const char *pName) : MapObjActor(pName) {
	mItemType = -1;
}

void DeadLeaves::init(const JMapInfoIter &rIter) {
	MapObjActor::init(rIter);
	MapObjActorInitInfo initInfo;
	MapObjActorUtil::setupInitInfoSimpleMapObj(&initInfo);

	initInfo.setupHitSensor();
	initInfo.setupHitSensorParam(8, 70.0f, TVec3f(0.0f, 30.0f, 0.0f));
	initInfo.setupNerve(&NrvDeadLeaves::NrvWait::sInstance);

	initialize(rIter, initInfo);

	MR::getJMapInfoArg0NoInit(rIter, &mItemType);
	MR::useStageSwitchAwake(this, rIter);
	MR::useStageSwitchWriteA(this, rIter);
	
	if (mItemType == 0)
		MR::declareCoin(this, 1);
	else if (mItemType == 1)
		MR::declareStarPiece(this, 3);
}

bool DeadLeaves::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
	if (MR::isMsgPlayerSpinAttack(msg)) {
		setNerve(&NrvDeadLeaves::NrvSpin::sInstance);
		return true;
	}

	return false;
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
	void NrvWait::execute(Spine *pSpine) const {}

	void NrvSpin::execute(Spine *pSpine) const {
		DeadLeaves* pActor = (DeadLeaves*)pSpine->mExecutor;
		pActor->exeSpin();
	}

	NrvWait(NrvWait::sInstance);
	NrvSpin(NrvSpin::sInstance);
}
