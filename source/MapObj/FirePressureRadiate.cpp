#include "spack/MapObj/FirePressureRadiate.h"
#include "spack/Util.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: FirePressureRadiate
* Parameters:
*  - Obj_arg0, float, 0.0f: Cannon rotation
*  - Obj_arg1, long, 300: Wait time
*  - Obj_arg2, long, 300: Shoot time
*  - cutscene
*  - SW_A, use, read: Enables flame shooting
*  - SW_APPEAR
*  - SW_AWAKE
* 
* A variant of the fire ball shooters found throughout the game. It originally appeared in several
* SMG1 levels, including Toy Time Galaxy and Bubble Blast Galaxy. This object shoots a flame that
* burns Mario upon contact.
*/

FirePressureRadiate::FirePressureRadiate(const char *pName) : LiveActor(pName) {
	mJointCtrl = NULL;
	mCannonRotation = 0.0f;
	mWaitTime = 300;
	mRadiateTime = 300;
	mRadiateSteps = -1;
	mFlameLength = 0.0f;
	mFlameLengthMax = 500.0f;
	mMsgGroup = NULL;
	mGroupLeader = false;

	PSMTXIdentity((MtxPtr)&mRadiateEffectMtx);
}

void FirePressureRadiate::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	MR::processInitFunction(this, "FirePressure", false);
	MR::connectToSceneNoShadowedMapObjStrongLight(this);
	
	// Initialize sensors
	initHitSensor(3);
	MR::addHitSensorMapObj(this, "Body", 8, 70.0f, TVec3f(0.0, 30.0f, 0.0f));
	MR::addHitSensorAtJointMapObj(this, "Cannon", "Cannon1", 8, 70.0f, TVec3f(0.0f, 40.0f, 0.0f));
	MR::addHitSensorCallbackEnemyAttack(this, "Radiate", 8, 50.0f);
	MR::invalidateHitSensor(this, "Radiate");

	// Initialize effects
	initEffectKeeper(0, NULL, false);
	MR::setEffectHostMtx(this, "Fire", (MtxPtr)&mRadiateEffectMtx);
	MR::setEffectHostMtx(this, "FireInd", (MtxPtr)&mRadiateEffectMtx);

	// Initialize sounds
	initSound(4, "FirePressure", false, TVec3f(0.0f, 0.0f, 0.0f));

	// Retrieve Obj_args
	MR::getJMapInfoArg0NoInit(rIter, &mCannonRotation);
	MR::getJMapInfoArg1NoInit(rIter, &mWaitTime);
	MR::getJMapInfoArg2NoInit(rIter, &mRadiateTime);

	// Setup joint controller for cannon rotation (Obj_arg0)
	JointControlDelegator<FirePressureRadiate>* jointCtrl = new JointControlDelegator<FirePressureRadiate>();
	jointCtrl->mObjPtr = this;
	jointCtrl->mCalcJointMtxFunc = &FirePressureRadiate::calcJointCannon;
	jointCtrl->mCalcJointMtxAfterChildFunc = NULL;

	MR::setJointControllerParam(jointCtrl, this, "Cannon1");
	mJointCtrl = jointCtrl;

	MR::initJointTransform(this);

	// Enable gravity calculation
	MR::calcGravity(this);

	// Initialize syncing group
	MR::setGroupClipping(this, rIter, 16);
	mMsgGroup = (MsgSharedGroup*)MR::joinToGroupArray(this, rIter, "ファイアプレッシャー（放射）軍団", 16);

	// Try register demo functor
	if (MR::tryRegisterDemoCast(this, rIter)) {
		MR::tryRegisterDemoActionFunctor(
			this,
			SPack::createFunctor(this, &FirePressureRadiate::startRelax),
			NULL
		);
	}

	// Try register SW_A functors
	if (MR::useStageSwitchWriteA(this, rIter)) {
		MR::listenStageSwitchOnOffA(
			this,
			SPack::createFunctor(this, &FirePressureRadiate::startWait),
			SPack::createFunctor(this, &FirePressureRadiate::startRelax)
		);

		initNerve(&NrvFirePressureRadiate::NrvRelax::sInstance, 0);
	}
	else 
		initNerve(&NrvFirePressureRadiate::NrvWait::sInstance, 0);

	// Setup SW_AWAKE and try register SW_APPEAR
	MR::useStageSwitchAwake(this, rIter);
	
	if (MR::useStageSwitchReadAppear(this, rIter))
		makeActorDead();
	else
		makeActorAppeared();
}

/*
* Determines if this FirePressureRadiate instance has the longest wait time in the same object
* group. If so, this instance becomes the "group leader". This means that once this object's
* wait time passes, all other FirePressureRadiate instances will start as well.
*/
void FirePressureRadiate::initAfterPlacement() {
	if (mMsgGroup) {
		LiveActorGroup* group = MR::getGroupFromArray(this);
		FirePressureRadiate* groupLeaderActor = (FirePressureRadiate*)group->getActor(0);

		for (u16 i = 1 ; i < group->mNumObjs; ++i) {
			FirePressureRadiate* otherGroupActor = (FirePressureRadiate*)group->getActor(i);

			if (groupLeaderActor->mWaitTime <= otherGroupActor->mWaitTime)
				groupLeaderActor = otherGroupActor;
		}

		mGroupLeader = groupLeaderActor == this;
	}
}

/*
* Tries to calculate the flame's length if the radiate steps is not set to -1.
*/
void FirePressureRadiate::control() {
	if (mRadiateSteps != -1) {
		if (mRadiateSteps < 70) {
			mFlameLength = MR::getLinerValue(mRadiateSteps / 70.0f, 51.0f, mFlameLengthMax, 1.0f);
			mRadiateSteps++;
		}
		else {
			mRadiateSteps = -1;
			mFlameLength = mFlameLengthMax;
		}
	}
}

void FirePressureRadiate::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	mJointCtrl->registerCallBack();
}

/*
* Calculates the sensor's position along a line relative to the player's current position.
*/
void FirePressureRadiate::updateHitSensor(HitSensor *pSensor) {
	TVec3f frontVec, position, lineStart, lineEnd;

	MR::extractMtxXDir((MtxPtr)&mRadiateEffectMtx, &frontVec);
	MR::extractMtxTrans((MtxPtr)&mRadiateEffectMtx, &position);
	JMAVECScaleAdd((Vec*)&frontVec, (Vec*)&position, (Vec*)&lineStart, 50.0f);
	JMAVECScaleAdd((Vec*)&frontVec, (Vec*)&lineStart, (Vec*)&lineEnd, mFlameLength - 50.0f);

	MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), lineStart, lineEnd);
}

void FirePressureRadiate::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	// Does player or ride touch fire sensor?
	if (MR::isSensorEnemyAttack(pSender)) {
		if (MR::isSensorPlayerOrRide(pReceiver))
			MR::sendMsgEnemyAttackFire(pReceiver, pSender);
	}
	// Does player or enemy touch nozzle?
	else if (MR::isSensorMapObj(pSender)) {
		if (MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver))
			MR::sendMsgPush(pReceiver, pSender);
	}
}

bool FirePressureRadiate::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
	if (MR::isMsgGroupMoveStop(msg)) {
		setNerve(&NrvFirePressureRadiate::NrvSyncWait::sInstance);
		return true;
	}
	else if (MR::isMsgGroupMoveStart(msg)) {
		setNerve(&NrvFirePressureRadiate::NrvWait::sInstance);
		return true;
	}

	return false;
}

void FirePressureRadiate::exeRelax() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "FireShotStart");
		MR::setBckFrame(this, 0.0f);
		MR::forceDeleteEffectAll(this);
		MR::invalidateHitSensor(this, "Radiate");
	}
}

void FirePressureRadiate::exeSyncWait() {
	if (mGroupLeader && MR::isStep(this, 60))
		mMsgGroup->sendMsgToGroupMember(ACTMES_GROUP_MOVE_START, getSensor("Body"), "Body");
}

void FirePressureRadiate::exeWait() {
	if (MR::isStep(this, mWaitTime))
		setNerve(&NrvFirePressureRadiate::NrvPrepareToRadiate::sInstance);
}

void FirePressureRadiate::exePrepareToRadiate() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "FireShotStart");

	if (MR::isStep(this, 34))
		setNerve(&NrvFirePressureRadiate::NrvRadiate::sInstance);
}

void FirePressureRadiate::exeRadiate() {
	MR::startLevelSound(this, "OjLvFPressureRadiate", -1, -1, -1);

	if (MR::isBckOneTimeAndStopped(this)) {
		JMath::gekko_ps_copy12(&mRadiateEffectMtx, MR::getJointMtx(this, "Cannon3"));
		MR::startAction(this, "FireShot");
	}

	if (MR::isStep(this, 25)) {
		MR::validateHitSensor(this, "Radiate");
		mRadiateSteps = 0;
	}

	if (MR::isGreaterEqualStep(this, mRadiateTime))
		setNerve(&NrvFirePressureRadiate::NrvRadiateMargin::sInstance);
}

void FirePressureRadiate::exeRadiateMargin() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "FireShotEnd");

	if (MR::isStep(this, 50)) {
		MR::invalidateHitSensor(this, "Radiate");

		if (mMsgGroup)
			setNerve(&NrvFirePressureRadiate::NrvSyncWait::sInstance);
		else
			setNerve(&NrvFirePressureRadiate::NrvWait::sInstance);
		
		if (mGroupLeader)
			mMsgGroup->sendMsgToGroupMember(ACTMES_GROUP_MOVE_STOP, getSensor("Body"), "Body");
	}
}

void FirePressureRadiate::startWait() {
	if (isNerve(&NrvFirePressureRadiate::NrvRelax::sInstance))
		setNerve(&NrvFirePressureRadiate::NrvWait::sInstance);
}

void FirePressureRadiate::startRelax() {
	if (!isNerve(&NrvFirePressureRadiate::NrvRelax::sInstance))
		setNerve(&NrvFirePressureRadiate::NrvRelax::sInstance);
}

/*
* Rotates the cannon joint's matrix around the Z-axis as specified by mCannonRotation. Compared to
* SMG1, this is much shorter as we use MR::tmpMtxRotZDeg to calculate the rotation offset instead
* of calculating it manually.
*/
bool FirePressureRadiate::calcJointCannon(TPositionMtx *pJointMtx, const JointControllerInfo &) {
	PSMTXConcat((MtxPtr)pJointMtx, (MtxPtr)MR::tmpMtxRotZDeg(mCannonRotation), (MtxPtr)pJointMtx);
	return true;
}

namespace NrvFirePressureRadiate {
	void NrvRelax::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exeRelax();
	}

	void NrvSyncWait::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exeSyncWait();
	}

	void NrvWait::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exeWait();
	}

	void NrvPrepareToRadiate::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exePrepareToRadiate();
	}

	void NrvRadiate::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exeRadiate();
	}

	void NrvRadiateMargin::execute(Spine *pSpine) const {
		FirePressureRadiate* pActor = (FirePressureRadiate*)pSpine->mExecutor;
		pActor->exeRadiateMargin();
	}

	NrvRelax(NrvRelax::sInstance);
	NrvSyncWait(NrvSyncWait::sInstance);
	NrvWait(NrvWait::sInstance);
	NrvPrepareToRadiate(NrvPrepareToRadiate::sInstance);
	NrvRadiate(NrvRadiate::sInstance);
	NrvRadiateMargin(NrvRadiateMargin::sInstance);
}