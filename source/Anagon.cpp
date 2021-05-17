#include "spack/Actor/Anagon.h"
#include "spack/SPackUtil.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorInitUtil.h"
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
#include "Util/StarPointerUtil.h"
#include "Util/SupportTicoUtil.h"

/*
* Created by Aurum
* 
* This is a simplified port of SkeletalFishBaby, the swimming Gringills seen in Drip Drop Galaxy or
* Deep Dark Galaxy. As the SMG1 object has a lot of complicated code related to the Kingfin boss
* fight, path changing as well as some joint controlling, I attempted to rewrite it entirely to get
* rid of the unnecessarily complicated code. Hence, this isn't a 1:1 port. The name is different
* because Anagon is the real Japanese name for Gringills. Interestingly, sound data for this enemy
* was already present in ActionSound under that name, suggesting that Gringills were meant to appear
* in the game at some point.
*/
Anagon::Anagon(const char* pName) : LiveActor(pName) {
	for (s32 i = 0; i < 3; i++)
		mStarPointerTargets[i] = NULL;

	mAnimScaleCtrl = new AnimScaleController(NULL);
}

void Anagon::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);

	initModelManagerWithAnm("Anagon", NULL);
	initNerve(&NrvAnagon::NrvSwim::sInstance, 0);
	MR::setClippingTypeSphere(this, 1000.0f);
	MR::addToAttributeGroupSearchTurtle(this);
	MR::connectToSceneEnemy(this);

	// Initialize RailRider
	initRailRider(rIter);
	SPack::initPosRotFromRailRider(this);

	f32 speed = 20.0f;
	MR::getJMapInfoArg0NoInit(rIter, &speed);
	mRailRider->setSpeed(speed);

	// Initialize hit sensors
	initHitSensor(4);
	MR::addHitSensorAtJointEnemy(this, "head", "Joint00", 16, 190.0f, TVec3f(0.0f, 20.0f, 100.0f));
	MR::addHitSensorAtJointEnemy(this, "bust", "Joint01", 16, 120.0f, TVec3f(0.0f, 0.0f, 30.0f));
	MR::addHitSensorAtJointEnemy(this, "waist", "Joint02", 16, 120.0f, TVec3f(0.0f, 0.0f, 20.0f));
	MR::addHitSensorAtJointEnemy(this, "tail", "Joint03", 16, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

	MR::initLightCtrl(this);
	initSound(6, "Anagon", false, TVec3f(0.0f, 0.0f, 0.0f));
	MR::initShadowFromCSV(this, "Shadow", false);
	MR::setGroupClipping(this, rIter, 32);
	initEffectKeeper(0, NULL, false);

	MR::useStageSwitchWriteDead(this, rIter);
	MR::declareStarPiece(this, 10);
	MR::onCalcGravity(this);

	// Initialize Star Pointer targets, SMG1 uses an array of four targets
	for (s32 i = 0; i < 4; i++) {
		LiveActor* target = this;
		if (i > 0) {
			target = new LiveActor("StarPointerTargetDummy");
			mStarPointerTargets[i - 1] = target;
			target->makeActorAppeared();
		}

		TVec3f targetOffset((TVec3f&)sStarPointerTargetOffset[i]);
		f32 targetSize = sStarPointerTargetSize[i];
		const char* targetJoint = sStarPointerTargetJoint[i];

		Mtx* jointMtx = (Mtx*)MR::getJointMtx(this, targetJoint);
		target->initActorStarPointerTarget(targetSize, NULL, (Mtx4*)jointMtx, targetOffset);
	}

	if (MR::useStageSwitchReadAppear(this, rIter)) {
		MR::syncStageSwitchAppear(this);
		makeActorDead();
	}
	else
		makeActorAppeared();
}

void Anagon::makeActorAppeared() {
	LiveActor::makeActorAppeared();

	for (s32 i = 0; i < 3; i++)
		mStarPointerTargets[i]->makeActorAppeared();

	MR::startLevelSound(this, "BmSklFishBabyAppear", -1, -1, -1);
	MR::startBck(this, "Swim");
	setNerve(&NrvAnagon::NrvSwim::sInstance);
	MR::validateClipping(this);
}

void Anagon::kill() {
	LiveActor::kill();

	for (s32 i = 0; i < 3; i++)
		mStarPointerTargets[i]->makeActorDead();

	MR::startLevelSound(this, "EmExplodeUnderWater", -1, -1, -1);
	
	if (MR::isValidSwitchDead(this))
		MR::onSwitchDead(this);
}

void Anagon::control() {
	mAnimScaleCtrl->updateNerve();
}

void Anagon::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	MR::updateBaseScale(this, mAnimScaleCtrl);
}

void Anagon::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isSensorPlayerOrRide(pHit2)) {
		if (isNerve(&NrvAnagon::NrvSwim::sInstance) && MR::sendMsgEnemyAttack(pHit2, pHit1))
			MR::shakeCameraStrong();
		else
			MR::sendMsgPush(pHit2, pHit1);
	}
}

u32 Anagon::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
		if (isNerve(&NrvAnagon::NrvSwim::sInstance) || isNerve(&NrvAnagon::NrvBind::sInstance)) {
			MR::shakeCameraNormal();
			setNerve(&NrvAnagon::NrvBreak::sInstance);
			return 1;
		}
	}
	else if (MR::isMsgStarPieceReflect(msg)) {
		mAnimScaleCtrl->startHitReaction();
		return 1;
	}

	return 0;
}

void Anagon::exeSwim() {
	if (MR::isFirstStep(this))
		MR::startBck(this, "Swim");

	MR::startLevelSound(this, "EmLvSnakefishSwim", -1, -1, -1);

	if (isStarPointerPointing())
		setNerve(&NrvAnagon::NrvBind::sInstance);

	mRailRider->move();
	SPack::updatePosRotFromRailRider(this);
}

void Anagon::exeBind() {
	if (MR::isFirstStep(this)) {
		mAnimScaleCtrl->startDpdHitVibration();
		MR::emitEffect(this, "StarPointerHold");
	}

	MR::startDPDFreezeLevelSound(this);

	if (!isStarPointerPointing())
		setNerve(&NrvAnagon::NrvSwim::sInstance);
}

void Anagon::endBind() {
	MR::tryDeleteEffect(this, "StarPointerHold");
	mAnimScaleCtrl->startAnim();
}

void Anagon::exeBreak() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Death");
		MR::startLevelSound(this, "EmExplodeUnderWater", -1, -1, -1);
		MR::startLevelSound(this, "EvSnakeheadDamage", -1, -1, -1);
		MR::invalidateClipping(this);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvAnagon::NrvDead::sInstance);
}

void Anagon::exeDead() {
	if (MR::isFirstStep(this)) {
		TVec3f vec;
		MR::copyJointPos(this, "Center", &vec);
		MR::appearStarPiece(this, vec, 10, 10.0f, 40.0f, false);
		MR::startLevelSound(this, "OjStarPieceBurstWF", -1, -1, -1);
		kill();
	}
}

bool Anagon::isStarPointerPointing() {
	for (s32 i = 0; i < 4; i++) {
		LiveActor* target = this;
		if (i > 0)
			target = mStarPointerTargets[i - 1];

		if (MR::attachSupportTicoToTarget(target))
			return true;
	}

	return false;
}

namespace NrvAnagon {
	void NrvSwim::execute(Spine* spine) const {
		Anagon* pActor = (Anagon*)spine->mExecutor;
		pActor->exeSwim();
	}

	void NrvBind::execute(Spine* spine) const {
		Anagon* pActor = (Anagon*)spine->mExecutor;
		pActor->exeBind();
	}

	void NrvBind::executeOnEnd(Spine* spine) const {
		Anagon* pActor = (Anagon*)spine->mExecutor;
		pActor->endBind();
	}

	void NrvBreak::execute(Spine* spine) const {
		Anagon* pActor = (Anagon*)spine->mExecutor;
		pActor->exeBreak();
	}

	void NrvDead::execute(Spine* spine) const {
		Anagon* pActor = (Anagon*)spine->mExecutor;
		pActor->exeDead();
	}

	NrvSwim(NrvSwim::sInstance);
	NrvBind(NrvBind::sInstance);
	NrvBreak(NrvBreak::sInstance);
	NrvDead(NrvDead::sInstance);
}
