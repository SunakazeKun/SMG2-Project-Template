#include "spack/Enemy/Anagon.h"
#include "spack/Util.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: Anagon
* Parameters:
* - Obj_arg0, float, 20.0f: Speed
* - SW_AWAKE
* - SW_APPEAR, use
* - SW_DEAD, use, write: Activated when killed
* 
* This is a simplified port of SkeletalFishBaby, the swimming Gringills from Drip Drop Galaxy and
* Deep Dark Galaxy. As this actor has a lot of complicated code related to the Kingfin boss fight,
* path changing as well as some joint controlling, I attempted to rewrite it entirely. Hence, this
* isn't a 1:1 port. The name is different because Anagon is the real Japanese name for Gringills.
* Interestingly, sound data for this enemy was already present in ActionSound under that name,
* suggesting that Gringills were meant to appear in the game at some point.
*/

Anagon::Anagon(const char *pName) : LiveActor(pName) {
	for (s32 i = 0; i < 3; i++)
		mStarPointerTargets[i] = NULL;

	mAnimScaleCtrl = NULL;
}

void Anagon::init(const JMapInfoIter &rIter) {
	MR::processInitFunction(this, rIter, false);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemy(this);

	// Initialize RailRider
	initRailRider(rIter);
	SPack::initPosRotFromRailRider(this);

	f32 speed = 20.0f;
	MR::getJMapInfoArg0NoInit(rIter, &speed);
	mRailRider->setSpeed(speed);

	// Initialize hit sensors
	initHitSensor(4);
	MR::addHitSensorAtJointEnemy(this, "Head", "Joint00", 16, 190.0f, TVec3f(0.0f, 20.0f, 100.0f));
	MR::addHitSensorAtJointEnemy(this, "Bust", "Joint01", 16, 120.0f, TVec3f(0.0f, 0.0f, 30.0f));
	MR::addHitSensorAtJointEnemy(this, "Waist", "Joint02", 16, 120.0f, TVec3f(0.0f, 0.0f, 20.0f));
	MR::addHitSensorAtJointEnemy(this, "Tail", "Joint03", 16, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

	initSound(6, "Anagon", false, TVec3f(0.0f, 0.0f, 0.0f));
	initEffectKeeper(0, NULL, false);
	MR::initLightCtrl(this);
	MR::initShadowFromCSV(this, "Shadow", false);
	MR::setGroupClipping(this, rIter, 32);
	MR::setClippingTypeSphere(this, 1000.0f);
	MR::addToAttributeGroupSearchTurtle(this);
	MR::declareStarPiece(this, 10);
	MR::onCalcGravity(this);

	MR::useStageSwitchWriteDead(this, rIter);
	MR::useStageSwitchAwake(this, rIter);

	// Initialize Star Pointer targets, SMG1 uses an array of four targets
	mAnimScaleCtrl = new AnimScaleController(NULL);

	for (s32 i = 0; i < 4; i++) {
		LiveActor* target = this;
		if (i > 0) {
			target = new LiveActor("StarPointerTargetDummy");
			mStarPointerTargets[i - 1] = target;
		}

		TVec3f targetOffset(sStarPointerTargetOffset[i]);
		f32 targetSize = sStarPointerTargetSize[i];
		const char* targetJoint = sStarPointerTargetJoint[i];
		MtxPtr jointMtx = (MtxPtr)MR::getJointMtx(this, targetJoint);

		target->initActorStarPointerTarget(targetSize, NULL, jointMtx, targetOffset);
	}

	initNerve(&NrvAnagon::NrvSwim::sInstance, 0);
	MR::useStageSwitchSyncAppear(this, rIter);
}

void Anagon::makeActorAppeared() {
	LiveActor::makeActorAppeared();

	for (s32 i = 0; i < 3; i++)
		mStarPointerTargets[i]->makeActorAppeared();

	MR::startLevelSound(this, "BmSklFishBabyAppear", -1, -1, -1);
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
	mAnimScaleCtrl->update();
}

void Anagon::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	MR::updateBaseScale(this, mAnimScaleCtrl);
}

void Anagon::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isSensorPlayerOrRide(pReceiver)) {
		if (isNerve(&NrvAnagon::NrvSwim::sInstance) && MR::sendMsgEnemyAttack(pReceiver, pSender))
			MR::shakeCameraStrong();
		else
			MR::sendMsgPush(pReceiver, pSender);
	}
}

bool Anagon::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
		if (isNerve(&NrvAnagon::NrvSwim::sInstance) || isNerve(&NrvAnagon::NrvBind::sInstance)) {
			MR::shakeCameraNormal();
			setNerve(&NrvAnagon::NrvBreak::sInstance);
			return true;
		}
	}
	else if (MR::isMsgStarPieceReflect(msg)) {
		mAnimScaleCtrl->startHitReaction();
		return true;
	}

	return false;
}

void Anagon::exeSwim() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "Swim");

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
		MR::startAction(this, "Death");
		MR::startLevelSound(this, "EmExplodeUnderWater", -1, -1, -1);
		MR::startLevelSound(this, "EvSnakeheadDamage", -1, -1, -1);
		MR::invalidateClipping(this);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvAnagon::NrvDead::sInstance);
}

void Anagon::exeDead() {
	if (MR::isFirstStep(this)) {
		TVec3f starPiecePos;
		MR::copyJointPos(this, "Center", &starPiecePos);
		MR::appearStarPiece(this, starPiecePos, 10, 10.0f, 40.0f, false);
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
	void NrvSwim::execute(Spine *pSpine) const {
		Anagon* pActor = (Anagon*)pSpine->mExecutor;
		pActor->exeSwim();
	}

	void NrvBind::execute(Spine *pSpine) const {
		Anagon* pActor = (Anagon*)pSpine->mExecutor;
		pActor->exeBind();
	}

	void NrvBind::executeOnEnd(Spine *pSpine) const {
		Anagon* pActor = (Anagon*)pSpine->mExecutor;
		pActor->endBind();
	}

	void NrvBreak::execute(Spine *pSpine) const {
		Anagon* pActor = (Anagon*)pSpine->mExecutor;
		pActor->exeBreak();
	}

	void NrvDead::execute(Spine *pSpine) const {
		Anagon* pActor = (Anagon*)pSpine->mExecutor;
		pActor->exeDead();
	}

	NrvSwim(NrvSwim::sInstance);
	NrvBind(NrvBind::sInstance);
	NrvBreak(NrvBreak::sInstance);
	NrvDead(NrvDead::sInstance);
}
