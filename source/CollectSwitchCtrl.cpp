#include "spack/Actor/CollectSwitchCtrl.h"
#include "System/GameDataFunction.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/EventUtil.h"
#include "Util/JMapUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SceneUtil.h"

/*
* Created by Aurum
* 
* This is a switch controller that activates an event when the specified amount of the specified
* item is collected. It can be used to check for Coins, Purple Coins, Star Bits and Power Stars.
*/
CollectSwitchCtrl::CollectSwitchCtrl(const char* pName) : NameObj(pName) {
	mStageSwitchCtrl = NULL;
	mItem = 0;
	mCount = 0;
	mComparator = 0;
	mSwitchOnce = false;
}

void CollectSwitchCtrl::init(const JMapInfoIter& rIter) {
	MR::connectToSceneMapObjMovement(this);
	MR::getJMapInfoArg0NoInit(rIter, &mItem);
	MR::getJMapInfoArg1NoInit(rIter, &mCount);
	MR::getJMapInfoArg2NoInit(rIter, &mComparator);
	MR::getJMapInfoArg3NoInit(rIter, &mSwitchOnce);

	bool clearStarPiece;
	MR::getJMapInfoArg4NoInit(rIter, &clearStarPiece);
	if (clearStarPiece && mItem == 2)
		MR::addStarPiece(-999);

	mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void CollectSwitchCtrl::movement() {
	// Fetch specified item count
	s32 count = 0;
	if (mItem == 0)
		count = MR::getCoinNum();
	else if (mItem == 1)
		count = MR::getPurpleCoinNum();
	else if (mItem == 2)
		count = MR::getStarPieceNum();
	else if (mItem == 3)
		count = GameDataFunction::calcCurrentPowerStarNum();

	// Compare actual count with expected count
	bool state = false;
	if (mComparator == 0)
		state = count == mCount;
	else if (mComparator == 1)
		state = count < mCount;
	else if (mComparator == 2)
		state = count <= mCount;
	else if (mComparator == 3)
		state = count > mCount;
	else if (mComparator == 4)
		state = count >= mCount;
	else if (mComparator == 5)
		state = count != mCount;

	// Set switch depending on result
	if (state)
		mStageSwitchCtrl->onSwitchA();
	else if (!mSwitchOnce)
		mStageSwitchCtrl->offSwitchA();
}
