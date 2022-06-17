#include "pt/MapObj/DeadLeaves.h"

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
* A pile of autumn leaves from Gold Leaf Galaxy. When using the Spin Attack in its range, it can spawn an item and
* activate a SW_A event as well.
*/

namespace pt {
	DeadLeaves::DeadLeaves(const char *pName) : LiveActor(pName) {
		mItemType = -1;
	}

	void DeadLeaves::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, false);

		// Initialize items
		MR::getJMapInfoArg0NoInit(rIter, &mItemType);

		if (mItemType == 0) {
			MR::declareCoin(this, 1);
		}
		else if (mItemType == 1) {
			MR::declareStarPiece(this, 3);
		}

		// Setup nerve and make appeared
		initNerve(&NrvDeadLeaves::NrvWait::sInstance, 0);
		makeActorAppeared();
	}

	bool DeadLeaves::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
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

			if (mItemType == 0) {
				MR::appearCoinPop(this, mTranslation, 1);
			}
			else if (mItemType == 1) {
				if (MR::appearStarPiece(this, mTranslation, 3, 10.0f, 40.0f, false)) {
					MR::startLevelSound(this, "OjStarPieceBurst", -1, -1, -1);
				}
			}

			if (MR::isValidSwitchA(this)) {
				MR::onSwitchA(this);
			}
		}

		if (MR::isActionEnd(this)) {
			setNerve(&NrvDeadLeaves::NrvWait::sInstance);
		}
	}

	namespace NrvDeadLeaves {
		void NrvWait::execute(Spine *pSpine) const {}

		void NrvSpin::execute(Spine *pSpine) const {
			DeadLeaves *pActor = (DeadLeaves*)pSpine->mExecutor;
			pActor->exeSpin();
		}

		NrvWait(NrvWait::sInstance);
		NrvSpin(NrvSpin::sInstance);
	}
}
