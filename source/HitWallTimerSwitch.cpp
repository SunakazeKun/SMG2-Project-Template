#include "spack/Actor/HitWallTimerSwitch.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/DemoUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"
#include "mtx.h"

/*
* Created by Aurum
* 
* This is one of many unused switch controllers from SMG1. It has a purple button which is can be
* activated using any of Mario's attacks. It fits 2D sections very well.
*/
HitWallTimerSwitch::HitWallTimerSwitch(const char* pName) : LiveActor(pName) {
	mMoveCollision = 0;
	mMapObjConnector = new MapObjConnector(this);
	mTimer = 300;
	mActivate = false;
}

void HitWallTimerSwitch::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("HitWallTimerSwitch", NULL);
	MR::connectToSceneMapObjDecoration(this);

	initHitSensor(2);
	MR::addHitSensorMapObj(this, "Body", 16, 50.0, TVec3f(0.0f, 0.0f, 0.0f));
	MR::addHitSensor(this, "Hit", 0x73, 16, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));

	MR::initCollisionParts(this, "HitWallTimerSwitch", getSensor("Body"), NULL);
	mMoveCollision = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("Hit"), MR::CollisionScaleType_2);

	initSound(4, "HitWallTimerSwitch", false, TVec3f(0.0f, 0.0f, 0.0f));
	
	MR::useStageSwitchWriteA(this, rIter);
	MR::getJMapInfoArg0NoInit(rIter, &mTimer);
	MR::startBck(this, "Wait");

	initNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance, 0);

	makeActorAppeared();
}

void HitWallTimerSwitch::initAfterPlacement() {
	mMapObjConnector->attachToBack();
}

void HitWallTimerSwitch::calcAnim() {
	LiveActor::calcAnim();
	Mtx mtxCollision;
	PSMTXCopy((Mtx4*)MR::getJointMtx(this, "Move"), (Mtx4*)&mtxCollision);
	mMoveCollision->setMtx((TPositionMtx&)mtxCollision);
}

void HitWallTimerSwitch::control() {
	mActivate = false;
}

void HitWallTimerSwitch::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	mMapObjConnector->connect();
}

u32 HitWallTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgPlayerHitAll(msg) || MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerTrample(msg) || MR::isMsgStarPieceReflect(msg)) {
		mActivate = true;
		return !MR::isMsgPlayerHitAll(msg);
	}

	return 0;
}

u32 HitWallTimerSwitch::receiveOtherMsg(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (!isNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance))
		return 0;
	return msg == 0x99 && pHit2->isType(0x73);
}

void HitWallTimerSwitch::exeOff() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Wait");
		MR::validateClipping(this);
		MR::offSwitchA(this);
		MR::validateCollisionParts(mMoveCollision);
	}

	if (mActivate)
		setNerve(&NrvHitWallTimerSwitch::NrvSwitchDown::sInstance);
}

void HitWallTimerSwitch::exeSwitchDown() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "On");
		MR::startBtp(this, "On");
		MR::invalidateClipping(this);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvHitWallTimerSwitch::NrvOn::sInstance);
}

void HitWallTimerSwitch::exeOn() {
	if (MR::isFirstStep(this)) {
		MR::shakeCameraNormal();
		MR::onSwitchA(this);
		MR::startLevelSound(this, "OjSwitch1", -1, -1, -1);
		MR::invalidateCollisionParts(mMoveCollision);
	}

	s32 step = getNerveStep();

	if (step <= mTimer) {
		if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
			if (mTimer == step)
				MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
			else if ((step % 60) == 0) {
				if (step >= (mTimer - 120))
					MR::startSystemSE("SE_SY_TIMER_A_1", -1, -1);
				else
					MR::startSystemSE("SE_SY_TIMER_A_2", -1, -1);
			}
		}
	}
	else
		setNerve(&NrvHitWallTimerSwitch::NrvSwitchUp::sInstance);
}

void HitWallTimerSwitch::exeSwitchUp() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Off");
		MR::startBtp(this, "Off");
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvHitWallTimerSwitch::NrvOff::sInstance);
}

namespace NrvHitWallTimerSwitch {
	void NrvOff::execute(Spine* spine) const {
		HitWallTimerSwitch* pActor = (HitWallTimerSwitch*)spine->mExecutor;
		pActor->exeOff();
	}

	void NrvSwitchDown::execute(Spine* spine) const {
		HitWallTimerSwitch* pActor = (HitWallTimerSwitch*)spine->mExecutor;
		pActor->exeSwitchDown();
	}

	void NrvOn::execute(Spine* spine) const {
		HitWallTimerSwitch* pActor = (HitWallTimerSwitch*)spine->mExecutor;
		pActor->exeOn();
	}

	void NrvSwitchUp::execute(Spine* spine) const {
		HitWallTimerSwitch* pActor = (HitWallTimerSwitch*)spine->mExecutor;
		pActor->exeSwitchUp();
	}

	NrvOff(NrvOff::sInstance);
	NrvSwitchDown(NrvSwitchDown::sInstance);
	NrvOn(NrvOn::sInstance);
	NrvSwitchUp(NrvSwitchUp::sInstance);
}
