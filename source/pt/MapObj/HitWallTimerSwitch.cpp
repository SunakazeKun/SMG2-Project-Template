#include "pt/MapObj/HitWallTimerSwitch.h"

/*
* Author: Aurum
* Objects: HitWallTimerSwitch
* Parameters:
*  - Obj_arg0, long, 300: Activation time
*  - Obj_arg1, list, 0: Screen effect type
*   *: nothing
*   0: Switching screen effect (crashes due to missing model, kept for compatibility)
*   1: Time stop screen effect
*  - SW_A, need, write: Activated when hit
*  - SW_AWAKE
* 
* One of the many unused switch controllers from SMG1. It can be activated by attacking the purple
* button. It is intended to be used in 2D sections. For SMG2, compatibility with two screen effects
* has been added.
*/

namespace pt {
	HitWallTimerSwitch::HitWallTimerSwitch(const char *pName) : LiveActor(pName) {
		mMoveCollision = 0;
		mMapObjConnector = new MapObjConnector(this);
		mTimer = 300;
		mActivate = false;
		mScreenEffectType = 0;
	}

	void HitWallTimerSwitch::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, false);

		// Initialize miscellaneous attributes
		MR::invalidateClipping(this);
		MR::setSensorType(this, "Hit", ATYPE_SWITCH_BIND);
		mMoveCollision = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("Hit"), MR::CollisionScaleType_Default);

		MR::getJMapInfoArg0NoInit(rIter, &mTimer);
		MR::startAction(this, "Wait");
		MR::startBtp(this, "Off");

		// Initialize screen effect
		MR::getJMapInfoArg1NoInit(rIter, &mScreenEffectType);

		if (mScreenEffectType == 1) {
			MR::createSwitchingScreenEffect();
		}
		else if (mScreenEffectType == 2) {
			MR::createTimeStopScreenEffect();
		}

		// Setup nerve and make appeared
		initNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance, 0);
		makeActorAppeared();
	}

	void HitWallTimerSwitch::initAfterPlacement() {
		mMapObjConnector->attachToBack();
	}

	void HitWallTimerSwitch::calcAnim() {
		LiveActor::calcAnim();
		TMtx34f mtxCollision;
		PSMTXCopy(MR::getJointMtx(this, "Move"), mtxCollision);
		mMoveCollision->setMtx(mtxCollision);
	}

	void HitWallTimerSwitch::control() {
		mActivate = false;
	}

	void HitWallTimerSwitch::calcAndSetBaseMtx() {
		LiveActor::calcAndSetBaseMtx();
		mMapObjConnector->connect();
	}

	bool HitWallTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
		if (!isNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance)) {
			return false;
		}

		if (MR::isMsgPlayerHitAll(msg) || MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerTrample(msg) || MR::isMsgStarPieceReflect(msg)) {
			mActivate = true;
			return !MR::isMsgPlayerHitAll(msg);
		}

		return true;
	}

	bool HitWallTimerSwitch::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
		if (isNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance)) {
			return msg == ACTMES_IS_RUSH_ENABLE && pReceiver->isType(ATYPE_SWITCH_BIND);
		}

		return false;
	}

	void HitWallTimerSwitch::exeOff() {
		if (MR::isFirstStep(this)) {
			MR::startAction(this, "Wait");
			MR::validateClipping(this);
			MR::offSwitchA(this);
			MR::validateCollisionParts(mMoveCollision);
		}

		if (mActivate) {
			setNerve(&NrvHitWallTimerSwitch::NrvSwitchDown::sInstance);
		}
	}

	void HitWallTimerSwitch::exeSwitchDown() {
		if (MR::isFirstStep(this)) {
			MR::startAction(this, "On");
			MR::invalidateClipping(this);
		}

		if (MR::isActionEnd(this)) {
			setNerve(&NrvHitWallTimerSwitch::NrvOn::sInstance);
		}
	}

	void HitWallTimerSwitch::exeOn() {
		if (MR::isFirstStep(this)) {
			MR::shakeCameraNormal();
			MR::onSwitchA(this);
			MR::startActionSound(this, "OjSwitch1", -1, -1, -1);
			MR::invalidateCollisionParts(mMoveCollision);

			if (mScreenEffectType == 1) {
				MR::onSwitchingScreenEffect();
			}
			else if (mScreenEffectType == 2) {
				MR::onTimeStopScreenEffect();
			}
		}

		s32 step = getNerveStep();

		if (step <= mTimer) {
			if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
				if (mTimer == step) {
					MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
				}
				else if ((step % 60) == 0) {
					if (step >= (mTimer - 120)) {
						MR::startSystemSE("SE_SY_TIMER_A_1", -1, -1);
					}
					else {
						MR::startSystemSE("SE_SY_TIMER_A_2", -1, -1);
					}
				}
			}
		}
		else {
			setNerve(&NrvHitWallTimerSwitch::NrvSwitchUp::sInstance);
		}
	}

	void HitWallTimerSwitch::exeSwitchUp() {
		if (MR::isFirstStep(this)) {
			MR::startAction(this, "Off");

			if (mScreenEffectType == 1) {
				MR::offSwitchingScreenEffect();
			}
			else if (mScreenEffectType == 2) {
				MR::offTimeStopScreenEffect();
			}
		}

		if (MR::isActionEnd(this)) {
			setNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance);
		}
	}

	namespace NrvHitWallTimerSwitch {
		void NrvOff::execute(Spine *pSpine) const {
			HitWallTimerSwitch *pActor = (HitWallTimerSwitch*)pSpine->mExecutor;
			pActor->exeOff();
		}

		void NrvSwitchDown::execute(Spine *pSpine) const {
			HitWallTimerSwitch *pActor = (HitWallTimerSwitch*)pSpine->mExecutor;
			pActor->exeSwitchDown();
		}

		void NrvOn::execute(Spine *pSpine) const {
			HitWallTimerSwitch *pActor = (HitWallTimerSwitch*)pSpine->mExecutor;
			pActor->exeOn();
		}

		void NrvSwitchUp::execute(Spine *pSpine) const {
			HitWallTimerSwitch *pActor = (HitWallTimerSwitch*)pSpine->mExecutor;
			pActor->exeSwitchUp();
		}

		NrvOff(NrvOff::sInstance);
		NrvSwitchDown(NrvSwitchDown::sInstance);
		NrvOn(NrvOn::sInstance);
		NrvSwitchUp(NrvSwitchUp::sInstance);
	}
}
