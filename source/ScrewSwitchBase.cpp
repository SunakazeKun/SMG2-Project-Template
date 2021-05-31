#include "spack/Actor/ScrewSwitchBase.h"
#include "spack/SPackUtil.h"
#include "JMath/JMath.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/DemoUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/MtxUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SoundUtil.h"
#include "mtx.h"

/*
* Created by Aurum
* 
* This includes three screw switches from SMG1: ScrewSwitch, ScrewSwitchReverse and ValveSwitch.
* Since they all have nearly identical code, I made a base class for them to reduce the required
* amount of code space required. Furthermore, all of them now use SW_A, SW_AWAKE and support demo
* functionality as well.
* 
* This base class assumes that each screw switch has two hit sensors, Body and Binder.
*/
ScrewSwitchBase::ScrewSwitchBase(const char* pName) : LiveActor(pName) {
	mBindedActor = NULL;
	mMapObjConnector = new MapObjConnector(this);
}

void ScrewSwitchBase::initAfterPlacement() {
	mMapObjConnector->attachToUnder();
}

void ScrewSwitchBase::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	mMapObjConnector->connect();
}

u32 ScrewSwitchBase::receiveOtherMsg(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgRushBegin(msg) && MR::isSensorPlayer(pHit1) && MR::isOnPlayer(getSensor("Binder"))) {
		mBindedActor = pHit1->mParentActor;
		MR::startLevelSound(mBindedActor, "PvTwistStart", -1, -1, -1);
		MR::startLevelSound(mBindedActor, "PmSpinAttack", -1, -1, -1);
		setNerve(&NrvScrewSwitchBase::NrvAdjust::sInstance);
		return 1;
	}
	else if (MR::isMsgUpdateBaseMtx(msg) && mBindedActor && isNerve(&NrvScrewSwitchBase::NrvScrew::sInstance)) {
		updateBindActorMtx();
		return 1;
	}
	else
		return MR::isMsgRushCancel(msg);
}

void ScrewSwitchBase::exeEnd() {
	if (MR::isFirstStep(this)) {
		MR::invalidateHitSensors(this);

		if (MR::isPlayerInRush()) {
			MR::endBindAndPlayerWait(this);
			mBindedActor = NULL;
		}
	}
}

void ScrewSwitchBase::updateBindActorMtx() {
	Mtx mtxTR;
	PSMTXCopy((Mtx4*)mBindedActor->getBaseMtx(), (Mtx4*)&mtxTR);
	HitSensor* senBinder = getSensor("Binder");
	mtxTR[0][3] = senBinder->_4.x;
	mtxTR[1][3] = senBinder->_4.y;
	mtxTR[2][3] = senBinder->_4.z;
	MR::setBaseTRMtx(mBindedActor, (TPositionMtx&)mtxTR);
}

namespace NrvScrewSwitchBase {
	void NrvWait::execute(Spine* spine) const {
		ScrewSwitchBase* pActor = (ScrewSwitchBase*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvAdjust::execute(Spine* spine) const {
		ScrewSwitchBase* pActor = (ScrewSwitchBase*)spine->mExecutor;
		pActor->exeAdjust();
	}

	void NrvScrew::execute(Spine* spine) const {
		ScrewSwitchBase* pActor = (ScrewSwitchBase*)spine->mExecutor;
		pActor->exeScrew();
	}

	void NrvEnd::execute(Spine* spine) const {
		ScrewSwitchBase* pActor = (ScrewSwitchBase*)spine->mExecutor;
		pActor->exeEnd();
	}

	NrvWait(NrvWait::sInstance);
	NrvAdjust(NrvAdjust::sInstance);
	NrvScrew(NrvScrew::sInstance);
	NrvEnd(NrvEnd::sInstance);
}

ScrewSwitch::ScrewSwitch(const char* pName) : ScrewSwitchBase(pName) {
	mScrewCollision = NULL;
}

void ScrewSwitch::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("ScrewSwitch", NULL);
	MR::connectToSceneMapObjDecorationStrongLight(this);

	// Setup sensors
	initHitSensor(2);
	MR::addBodyMessageSensorMapObj(this);

	TVec3f offBinder;
	offBinder.scale(-150.0f, mGravity);
	MR::addHitSensorAtJoint(this, "Binder", "Screw", 0x5B, 8, 150.0f, offBinder);

	// Setup collision
	MR::initCollisionParts(this, "ScrewCol", getSensor("Binder"), (Mtx4*)MR::getJointMtx(this, "Screw"));
	mScrewCollision = MR::createCollisionPartsFromLiveActor(this, "ScrewReceiveCol", getSensor("Body"), (Mtx4*)MR::getJointMtx(this, "ScrewReceive"), MR::CollisionScaleType_1);
	MR::validateCollisionParts(mScrewCollision);

	MR::initLightCtrl(this);
	initEffectKeeper(0, NULL, false);
	initSound(4, "ScrewSwitch", false, TVec3f(0.0f, 0.0f, 0.0f));

	MR::useStageSwitchWriteA(this, rIter);
	MR::useStageSwitchAwake(this, rIter);
	MR::tryRegisterDemoCast(this, rIter);

	initNerve(&NrvScrewSwitchBase::NrvWait::sInstance, 0);

	makeActorAppeared();
}

void ScrewSwitch::control() {
	mScrewCollision->setMtx();
}

void ScrewSwitch::exeWait() {
	if (MR::isFirstStep(this))
		MR::startBrkAndSetFrameAndStop(this, "ScrewSwitchOn", 0.0f);
}

void ScrewSwitch::exeAdjust() {
	if (MR::isFirstStep(this))
		MR::startBrkAndSetFrameAndStop(this, "ScrewSwitchOn", 0.0f);

	f32 lerp = getNerveStep() / 3.0f;
	JMAVECLerp((Vec*)&mBindedActor->mTranslation, (Vec*)&getSensor("Binder")->_4, (Vec*)&mBindedActor->mTranslation, lerp);

	if (MR::isStep(this, 3))
		setNerve(&NrvScrewSwitchBase::NrvScrew::sInstance);
}

void ScrewSwitch::exeScrew() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "ScrewSwitchOn");
		MR::startBrk(this, "ScrewSwitchOn");
		MR::startActionPlayer("ScrewSwitchOn", NULL);
	}

	if (MR::isLessStep(this, 30))
		MR::startLevelSound(this, "OjLvScrewSwitchMove", -1, -1, -1);

	if (MR::isStep(this, 30)) {
		MR::startLevelSound(this, "OjScrewSwitchOn", -1, -1, -1);
		MR::tryRumblePadVeryStrong(this, 0);
		MR::shakeCameraNormal();
	}

	if (MR::isActionEnd(this)) {
		MR::onSwitchA(this);
		setNerve(&NrvScrewSwitchBase::NrvEnd::sInstance);
	}
}

ScrewSwitchReverse::ScrewSwitchReverse(const char* pName) : ScrewSwitchBase(pName) {
	mForceJump = false;
}

void ScrewSwitchReverse::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("ScrewSwitchReverse", NULL);
	MR::connectToSceneMapObjDecorationStrongLight(this);

	// Setup sensors
	initHitSensor(2);
	MR::addBodyMessageSensorMapObj(this);

	TVec3f offSensor;
	offSensor.scale(-130.0f, mGravity);
	MR::addHitSensorAtJoint(this, "Binder", "Screw", 0x5B, 8, 120.0f, offSensor);

	// Setup collision
	MR::initCollisionParts(this, "ScrewReverseCol", getSensor("Binder"), (Mtx4*)MR::getJointMtx(this, "Screw"));

	MR::initLightCtrl(this);
	initEffectKeeper(0, NULL, false);
	initSound(5, "ScrewSwitchReverse", false, TVec3f(0.0f, 0.0f, 0.0f));

	MR::useStageSwitchWriteA(this, rIter);
	MR::useStageSwitchAwake(this, rIter);
	MR::getJMapInfoArg0NoInit(rIter, &mForceJump);

	// Setup shadow
	f32 shadowLength = 1000.0f;
	MR::getJMapInfoArg7NoInit(rIter, &shadowLength);

	MR::initShadowVolumeCylinder(this, 100.0f);
	MR::setShadowDropPosition(this, NULL, mTranslation);
	MR::setShadowDropLength(this, NULL, shadowLength);

	initNerve(&NrvScrewSwitchBase::NrvWait::sInstance, 0);

	makeActorAppeared();
}

void ScrewSwitchReverse::exeWait() {}

void ScrewSwitchReverse::exeAdjust() {
	f32 lerp = getNerveStep() / 3.0f;
	JMAVECLerp((Vec*)&mBindedActor->mTranslation, (Vec*)&getSensor("Binder")->_4, (Vec*)&mBindedActor->mTranslation, lerp);

	if (MR::isStep(this, 3))
		setNerve(&NrvScrewSwitchBase::NrvScrew::sInstance);
}

void ScrewSwitchReverse::exeScrew() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "ScrewSwitchReverseOn");
		MR::startActionPlayer("ScrewSwitchReverseOn", NULL);
	}

	if (MR::isLessStep(this, 25))
		MR::startLevelSound(this, "OjLvScrSwitchRMove", -1, -1, -1);

	if (MR::isStep(this, 25)) {
		MR::startLevelSound(this, "OjScrSwitchROn", -1, -1, -1);
		MR::tryRumblePadVeryStrong(this, 0);
		MR::shakeCameraNormal();
		MR::invalidateHitSensors(this);
		MR::invalidateCollisionParts(this);

		if (MR::isPlayerInRush())
			endBind();
	}

	if (MR::isActionEnd(this)) {
		MR::startLevelSound(this, "EmExplodeS", -1, -1, -1);
		MR::onSwitchA(this);
		kill();
	}
}

void ScrewSwitchReverse::endBind() {
	TVec3f upVec;
	MR::calcUpVec(&upVec, this);
	TVec3f jumpVec;
	jumpVec.scale(28.0f, upVec);

	if (mForceJump)
		MR::endBindAndPlayerForceJump(this, jumpVec, 0);
	else
		MR::endBindAndPlayerJump(this, jumpVec, 0);

	mBindedActor = NULL;
}

ValveSwitch::ValveSwitch(const char* pName) : ScrewSwitchBase(pName) {
	PSMTXIdentity((Mtx4*)&mCollisionMtx);
	mIsReverse = false;
}

void ValveSwitch::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("ValveSwitch", NULL);
	MR::connectToSceneMapObjDecorationStrongLight(this);

	// Setup sensors
	initHitSensor(2);
	MR::addHitSensorMapObj(this, "Body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

	TVec3f offBinder;
	offBinder.scale(-75.0f, mGravity);
	MR::addHitSensorAtJoint(this, "Binder", "Valve", 0x5B, 8, 150.0f, offBinder);

	// Setup collision
	PSMTXCopy((Mtx4*)MR::getJointMtx(this, "Valve"), (Mtx4*)&mCollisionMtx);
	MR::initCollisionParts(this, "ValveCol", getSensor("Binder"), (Mtx4*)&mCollisionMtx);

	MR::initLightCtrl(this);
	initEffectKeeper(0, NULL, false);
	initSound(4, "ValveSwitch", false, TVec3f(0.0f, 0.0f, 0.0f));

	MR::useStageSwitchWriteA(this, rIter);
	MR::getJMapInfoArg0NoInit(rIter, &mIsReverse);

	initNerve(&NrvScrewSwitchBase::NrvWait::sInstance, 0);

	makeActorAppeared();
}

void ValveSwitch::control() {
	PSMTXCopy((Mtx4*)MR::getJointMtx(this, "Valve"), (Mtx4*)&mCollisionMtx);
	MR::makeMtxWithoutScale((TPositionMtx*)&mCollisionMtx, (TPositionMtx&)mCollisionMtx);
}

void ValveSwitch::exeWait() {
	if (MR::isFirstStep(this)) {
		MR::startBrk(this, "ValveSwitchOn");

		if (mIsReverse)
			MR::startAction(this, "ValveSwitchReverseOn");
		else
			MR::startAction(this, "ValveSwitchOn");

		MR::setBrkFrameAndStop(this, 0.0f);
		MR::setBckFrameAndStop(this, 0.0f);
	}
}

void ValveSwitch::exeAdjust() {
	if (MR::isFirstStep(this))
		MR::startBrkAndSetFrameAndStop(this, "ValveSwitchOn", 0.0f);

	f32 lerp = getNerveStep() / 3.0f;
	JMAVECLerp((Vec*)&mBindedActor->mTranslation, (Vec*)&getSensor("Binder")->_4, (Vec*)&mBindedActor->mTranslation, lerp);

	if (MR::isStep(this, 3))
		setNerve(&NrvScrewSwitchBase::NrvScrew::sInstance);
}

void ValveSwitch::exeScrew() {
	if (MR::isFirstStep(this)) {
		if (mIsReverse)
			MR::startAction(this, "ValveSwitchReverseOn");
		else
			MR::startAction(this, "ValveSwitchOn");

		MR::startBrk(this, "ValveSwitchOn");
		MR::startActionPlayer("ScrewSwitchOn", NULL);
	}

	if (MR::isLessStep(this, 30))
		MR::startLevelSound(this, "OjLvValveSwitchMove", -1, -1, -1);

	if (MR::isStep(this, 30)) {
		MR::startLevelSound(this, "OjValveSwitchOn", -1, -1, -1);
		MR::tryRumblePadVeryStrong(this, 0);
		MR::shakeCameraNormal();
	}

	if (MR::isActionEnd(this)) {
		MR::onSwitchA(this);
		setNerve(&NrvScrewSwitchBase::NrvEnd::sInstance);
	}
}
