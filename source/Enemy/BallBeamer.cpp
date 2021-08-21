#include "spack/Enemy/BallBeamer.h"
#include "spack/Util.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: BallBeamer
* Parameters:
*  - Obj_arg0, float, 12.0f: Ring speed
*  - Obj_arg1, long, 530: Ring life duration
*  - Obj_arg2, bool, false: Use shadow model?
*  - Obj_arg3, float, 0.0f: Color frame
*  - Obj_arg4, float, 3000.0f: Activation range
*  - Obj_arg5, long, 3: Max rings (capped between 3 and 11)
* 
* The spherical Ring Beamer from Buoy Base Galaxy. The code for ring beams to follow spherical
* gravity is left unused in SMG2, meaning that recreating this enemy wasn't very difficult at all.
* Like all ported enemies in this pack, this comes with proper Support Play behavior as well as
* SW_PARAM compatibility.
*/

BallBeamer::BallBeamer(const char *pName) : LiveActor(pName) {
	mBindStarPointer = NULL;
	mParamScale = NULL;
	mBeams = NULL;
	mNumMaxBeams = 3;
	mAttackDuration = 360;
	mActiveRange = 3000.0f;
	mActive = false;
}

void BallBeamer::init(const JMapInfoIter &rIter) {
	MR::processInitFunction(this, rIter, false);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemy(this);

	// Initialize sensors
	initHitSensor(1);
	MR::addHitSensorPush(this, "Body", 8, 120.0f, TVec3f(0.0f, -20.0f, 0.0f));

	initEffectKeeper(2, NULL, false);
	initSound(2, "BallBeamer", false, TVec3f(0.0f, 0.0f, 0.0f));
	MR::initLightCtrl(this);
	MR::initShadowVolumeSphere(this, 120.0f);

	initNerve(&NrvBallBeamer::NrvWait::sInstance, 0);
	makeActorAppeared();

	// Initialize Star Pointer
	MR::initStarPointerTarget(this, 200.0f, TVec3f(0.0f, 200.0f, 0.0f));
	mBindStarPointer = new WalkerStateBindStarPointer(this, NULL);

	// Initialize switch events
	MR::useStageSwitchAwake(this, rIter);

	if (MR::useStageSwitchReadA(this, rIter)) {
		MR::listenStageSwitchOnOffA(this,
			SPack::createFunctor(this, &BallBeamer::syncSwitchOnA),
			SPack::createFunctor(this, &BallBeamer::syncSwitchOffA)
		);
	}

	if (MR::useStageSwitchReadB(this, rIter)) {
		MR::listenStageSwitchOnB(this,
			SPack::createFunctor(this, &BallBeamer::syncSwitchOnB)
		);
	}

	// Initialize RingBeams
	f32 speed = 12.0f;
	s32 duration = 530;
	bool useShadow = false;
	f32 color = 0.0f;

	MR::getJMapInfoArg0NoInit(rIter, &speed);
	MR::getJMapInfoArg1NoInit(rIter, &duration);
	MR::getJMapInfoArg2NoInit(rIter, &useShadow);
	MR::getJMapInfoArg3NoInit(rIter, &color);
	MR::getJMapInfoArg4NoInit(rIter, &mActiveRange);
	MR::getJMapInfoArg5NoInit(rIter, &mNumMaxBeams);
	SPack::clamp<s32>(1, 11, &mNumMaxBeams);

	mParamScale = new ActorStateParamScale(this);
	mParamScale->init(rIter);

	// Initialize rings
	mBeams = new RingBeam*[mNumMaxBeams + 1];
	mAttackDuration = mNumMaxBeams * 120;

	for (s32 i = 0; i < mNumMaxBeams; i++) {
		RingBeam* beam = new RingBeam("リングビーム", this, true, useShadow, true, true, NULL);
		beam->initWithParamScale(rIter, mParamScale);
		beam->setSpeed(speed);
		beam->setLife(duration);
		beam->setColor(color);
		mBeams[i] = beam;
	}

	// Copy and store head matrix, then create head collision
	PSMTXCopy((MtxPtr)MR::getJointMtx(this, "Head"), mHeadMtx);
	MR::initCollisionParts(this, "Head", getSensor("Body"), mHeadMtx);
	MR::validateCollisionParts(this);
}

void BallBeamer::control() {
	mParamScale->update();

	if (mBindStarPointer->tryStartPointBind())
		setNerve(&NrvBallBeamer::NrvBind::sInstance);
}

void BallBeamer::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	MR::sendMsgPush(pReceiver, pSender);
}

bool BallBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
	return MR::isMsgStarPieceReflect(msg);
}

void BallBeamer::syncSwitchOnA() {
	mActive = true;
	setupAttack();
}

void BallBeamer::syncSwitchOffA() {
	mActive = false;
}

void BallBeamer::syncSwitchOnB() {
	MR::deleteEffect(this, "Charge");
	MR::emitEffect(this, "Vanish");
	makeActorDead();
}

void BallBeamer::setupAttack() {
	if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this))
		return;
	
	MR::invalidateClipping(this);

	if (!isNerve(&NrvBallBeamer::NrvAttack::sInstance))
		setNerve(&NrvBallBeamer::NrvAttack::sInstance);
}

void BallBeamer::exeWait() {
	if (mParamScale->isFirstStep()) {
		MR::validateClipping(this);
		MR::startAction(this, "Wait");
	}

	if (MR::isNearPlayer(this, mActiveRange))
		setupAttack();
}

void BallBeamer::exeAttack() {
	if (mParamScale->isGreaterEqualStep(mAttackDuration))
		setNerve(&NrvBallBeamer::NrvInter::sInstance);
	else {
		s32 step = mParamScale->getNerveStepRate();
		step -= (step / 120) * 120;

		if (step >= 75 && step < 119)
			MR::startLevelSound(this, "EmLvRingbeamCharge", -1, -1, -1);

		if (step == 75)
			MR::emitEffect(this, "Charge");

		if (step == 119) {
			MR::startAction(this, "Sign");
			MR::startLevelSound(this, "EmBallbeamerRotHead", -1, -1, -1);

			for (s32 i = 0; i < mNumMaxBeams; i++) {
				RingBeam* beam = mBeams[i];

				if (MR::isDead(beam)) {
					beam->appear();
					break;
				}
			}
		}
	}
}

void BallBeamer::exeInter() {
	if (mParamScale->isGreaterEqualStep(120)) {
		if (mActive)
			setNerve(&NrvBallBeamer::NrvAttack::sInstance);
		else
			setNerve(&NrvBallBeamer::NrvWait::sInstance);
	}
}

void BallBeamer::exeBind() {
	if (mParamScale->isFirstStep())
		MR::tryDeleteEffect(this, "Charge");
	MR::startDPDFreezeLevelSound(this);
	MR::updateActorStateAndNextNerve(this, (ActorStateBaseInterface*)mBindStarPointer, &NrvBallBeamer::NrvWait::sInstance);
}

namespace NrvBallBeamer {
	void NrvWait::execute(Spine *pSpine) const {
		BallBeamer* pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeWait();
	}

	void NrvAttack::execute(Spine *pSpine) const {
		BallBeamer* pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeAttack();
	}

	void NrvInter::execute(Spine *pSpine) const {
		BallBeamer* pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeInter();
	}

	void NrvBind::execute(Spine *pSpine) const {
		BallBeamer* pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeBind();
	}

	NrvWait(NrvWait::sInstance);
	NrvAttack(NrvAttack::sInstance);
	NrvInter(NrvInter::sInstance);
	NrvBind(NrvBind::sInstance);
}
