#include "spack/Actor/BallBeamer.h"
#include "spack/SPackUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/EffectUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"
#include "mtx.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

BallBeamer::BallBeamer(const char* pName) : LiveActor(pName) {
	// Instantiate nerves
	for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
		(*f)();

	mBeams = NULL;
	mNumMaxBeams = 3;
	mAttackDuration = 360;
	mActiveRange = 3000.0f;
	mActive = false;
}

void BallBeamer::init(const JMapInfoIter& rIter) {
	initModelManagerWithAnm("BallBeamer", NULL);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemy(this);
	MR::initLightCtrl(this);

	initHitSensor(1);
	MR::addHitSensorPush(this, "Body", 8, 120.0f, JGeometry::TVec3<f32>(0.0f, -20.0f, 0.0f));

	MR::initShadowVolumeSphere(this, 120.0f);
	initEffectKeeper(2, NULL, false);
	initSound(2, "BallBeamer", false, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

	initNerve(&NrvBallBeamer::NrvWait::sInstance, 0);
	makeActorAppeared();

	// Setup Switch event functors
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

	// Load Obj_args and initialize RingBeams
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
	SPack::clamp(1, 12, &mNumMaxBeams);

	// Initialize rings
	mBeams = new RingBeam*[mNumMaxBeams];
	mAttackDuration = mNumMaxBeams * 120;

	for (s32 i = 0; i < mNumMaxBeams; i++) {
		RingBeam* beam = new RingBeam("リングビーム", this, true, useShadow, true, true, NULL);
		beam->init(rIter);
		beam->setSpeed(speed);
		beam->setLife(duration);
		beam->setColor(color);
		mBeams[i] = beam;
	}

	// Copy and store head matrix, then create head collision
	PSMTXCopy(MR::getJointMtx(this, "Head")->val, mHeadMtx);
	MR::initCollisionParts(this, "Head", getSensor("Body"), mHeadMtx);
	MR::validateCollisionParts(this);
}

void BallBeamer::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
	MR::sendMsgPush(pHit2, pHit1);
}

u32 BallBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
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
	if (MR::isFirstStep(this)) {
		MR::validateClipping(this);
		MR::startBck(this, "Wait");
	}

	if (MR::isNearPlayer(this, mActiveRange))
		setupAttack();
}

void BallBeamer::exeAttack() {
	if (MR::isGreaterEqualStep(this, mAttackDuration))
		setNerve(&NrvBallBeamer::NrvInter::sInstance);
	else {
		s32 step = getNerveStep();
		step -= (step / 120) * 120;

		if (step >= 75 && step < 119)
			MR::startLevelSound(this, "EmLvRingbeamCharge", -1, -1, -1);

		if (step == 75)
			MR::emitEffect(this, "Charge");

		if (step == 119) {
			MR::startBck(this, "Sign");
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
	if (MR::isGreaterEqualStep(this, 120)) {
		if (mActive)
			setNerve(&NrvBallBeamer::NrvAttack::sInstance);
		else
			setNerve(&NrvBallBeamer::NrvWait::sInstance);
	}
}

namespace NrvBallBeamer {
	void NrvWait::execute(Spine* spine) const {
		BallBeamer* pActor = (BallBeamer*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvAttack::execute(Spine* spine) const {
		BallBeamer* pActor = (BallBeamer*)spine->mExecutor;
		pActor->exeAttack();
	}

	void NrvInter::execute(Spine* spine) const {
		BallBeamer* pActor = (BallBeamer*)spine->mExecutor;
		pActor->exeInter();
	}

	NrvWait(NrvWait::sInstance);
	NrvAttack(NrvAttack::sInstance);
	NrvInter(NrvInter::sInstance);
}
