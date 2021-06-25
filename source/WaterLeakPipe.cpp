#include "spack/Actor/WaterLeakPipe.h"
#include "spack/SPackUtil.h"
#include "JMath/JMath.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/EffectUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SoundUtil.h"
#include "mtx.h"

/*
* Created by Aurum
* 
* The splashing water pipe from Freezeflame Galaxy and Deep Dark Galaxy is a useful object for
* creating custom levels focused on Ice Flower platforming. If the player has the Ice power-up, it
* creates a unique ice platform on its spout. Like in SMG1, however, it doesn't come with its own
* collision so you may have to use a CollisionArea or bake the collision into the main model.
*/
IceStepNoSlip::IceStepNoSlip(Mtx4* mtx) : ModelObj("アイス床", "IceStepNoSlip", mtx, 0x1B, -2, -2, false) {}

void IceStepNoSlip::init(const JMapInfoIter & rIter) {
	ModelObj::init(rIter);

	initHitSensor(1);
	HitSensor* sensor = MR::addBodyMessageSensorMapObj(this);

	MR::initCollisionParts(this, "IceStepNoSlip", sensor, 0);
	initNerve(&NrvIceStepNoSlip::NrvAppear::sInstance, 0);
}

void IceStepNoSlip::appear() {
	LiveActor::appear();
	setNerve(&NrvIceStepNoSlip::NrvAppear::sInstance);
}

void IceStepNoSlip::exeAppear() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "Start");
}

void IceStepNoSlip::exeBreak() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "End");
	if (MR::isActionEnd(this))
		kill();
}

WaterLeakPipe::WaterLeakPipe(const char* pName) : LiveActor(pName) {
	mIceStep = NULL;
	mPipeLength = 500.0f;
	mIceStepMtx = NULL;
}

void WaterLeakPipe::init(const JMapInfoIter & rIter) {
	MR::initDefaultPos(this, rIter);
	
	MR::processInitFunction(this, rIter, false);
	mIceStepMtx = (Mtx*)MR::getJointMtx(this, "Top");

	MR::getJMapInfoArg0NoInit(rIter, &mPipeLength);
	initPipeHeight();

	MR::connectToSceneMapObjNoCalcAnim(this);

	initHitSensor(1);
	MR::addHitSensorAtJointMapObj(this, "ice", "Top", 8, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));

	initEffectKeeper(0, NULL, false);
	initSound(4, NULL, false, TVec3f(0.0f, 0.0f, 0.0f));

	// Calculate clipping offset and set clipping type
	TVec3f upVec;
	MR::calcUpVec(&upVec, this);
	JMAVECScaleAdd((Vec*)&upVec, (Vec*)&mTranslation, (Vec*)&mClippingOffset, 0.5f * mPipeLength);
	MR::setClippingTypeSphere(this, 0.5f * mPipeLength, &mClippingOffset);

	// Initialize ice platform
	mIceStep = new IceStepNoSlip((Mtx4*)mIceStepMtx);
	mIceStep->initWithoutIter();
	mIceStep->makeActorDead();

	initNerve(&NrvWaterLeakPipe::NrvWait::sInstance, 0);

	makeActorAppeared();
}

u32 WaterLeakPipe::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (!MR::isPlayerElementModeIce() || isNerve(&NrvWaterLeakPipe::NrvFreeze::sInstance))
		return 0;

	if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
		TVec3f upVec;
		MR::calcUpVec(&upVec, this);

		// Check if player is below water splash
		TVec3f playerOffset;
		playerOffset.sub(*MR::getPlayerPos(), mTranslation);
		f32 dot = playerOffset.dot(upVec);

		if (dot < 0.0f)
			return 0;

		// Check if player is above water splash
		dot = MR::getPlayerVelocity()->dot(upVec);

		if (dot >= 0.0f)
			return 0;

		// Finally set NrvFreeze
		setNerve(&NrvWaterLeakPipe::NrvFreeze::sInstance);
	}

	return 0;
}

void WaterLeakPipe::exeWait() {
	if (MR::isFirstStep(this))
		MR::emitEffect(this, "Splash");
	MR::startLevelSound(this, "OjLvWaterLeak", -1, -1, -1);
}

void WaterLeakPipe::exeFreeze() {
	if (MR::isFirstStep(this) && MR::isDead(mIceStep)) {
		MR::forceDeleteEffect(this, "Splash");
		mIceStep->appear();
		MR::startLevelSound(this, "OjIceFloorFreeze", -1, -1, -1);
		MR::invalidateClipping(this);
	}
	else if (MR::isOnPlayer(mIceStep) && MR::isPlayerElementModeIce())
		setNerve(&NrvWaterLeakPipe::NrvFreeze::sInstance);
	else if (MR::isStep(this, 15)) {
		mIceStep->setNerve(&NrvIceStepNoSlip::NrvBreak::sInstance);
		MR::startLevelSound(this, "OjIceFloorMelt", -1, -1, -1);
		MR::validateClipping(this);
		setNerve(&NrvWaterLeakPipe::NrvWait::sInstance);
	}
}

void WaterLeakPipe::initPipeHeight() {
	Mtx newIceStepMtx;
	PSMTXCopy((Mtx4*)mIceStepMtx, (Mtx4*)&newIceStepMtx);

	TVec3f upVec;
	TVec3f sizeVec;
	MR::calcUpVec(&upVec, this);
	JMAVECScaleAdd((Vec*)&upVec, (Vec*)&mTranslation, (Vec*)&sizeVec, mPipeLength);

	newIceStepMtx[0][3] = sizeVec.x;
	newIceStepMtx[1][3] = sizeVec.y;
	newIceStepMtx[2][3] = sizeVec.z;

	PSMTXCopy((Mtx4*)&newIceStepMtx, (Mtx4*)mIceStepMtx);
	calcAndSetBaseMtx();
}

namespace NrvIceStepNoSlip {
	void NrvAppear::execute(Spine* spine) const {
		IceStepNoSlip* pActor = (IceStepNoSlip*)spine->mExecutor;
		pActor->exeAppear();
	}

	void NrvBreak::execute(Spine* spine) const {
		IceStepNoSlip* pActor = (IceStepNoSlip*)spine->mExecutor;
		pActor->exeBreak();
	}

	NrvAppear(NrvAppear::sInstance);
	NrvBreak(NrvBreak::sInstance);
}

namespace NrvWaterLeakPipe {
	void NrvWait::execute(Spine* spine) const {
		WaterLeakPipe* pActor = (WaterLeakPipe*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvFreeze::execute(Spine* spine) const {
		WaterLeakPipe* pActor = (WaterLeakPipe*)spine->mExecutor;
		pActor->exeFreeze();
	}

	NrvWait(NrvWait::sInstance);
	NrvFreeze(NrvFreeze::sInstance);
}
