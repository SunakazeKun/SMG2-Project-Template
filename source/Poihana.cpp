#include "spack/Actor/Poihana.h"
#include "spack/SPackUtil.h"
#include "JGeometry/TUtil.h"
#include "JMath/JMath.h"
#include "Util/ActorAnimUtil.h"
#include "Util/ActorCameraUtil.h"
#include "Util/ActorInitUtil.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/ActorStateUtil.h"
#include "Util/EffectUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/MathUtil.h"
#include "Util/MtxUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SoundUtil.h"
#include "Util/StarPointerUtil.h"
#include "mtx.h"
#include "c_stdlib.h"

/*
* Created by Aurum and Someone
* 
* By far the most ambitious object in this pack and presumably one of the most requested ones. The
* Cataquacks from Gold Leaf Galaxy can be used in SMG2 now! This one was very difficult to port,
* but it also tought me a lot more about vector math in SMG.
*/
Poihana::Poihana(const char* pName) : LiveActor(pName) {
	mAnimScaleCtrl = NULL;
	mBindStarPointer = NULL;
	mCamInfo = NULL;
	mBindedActor = NULL;
	mLaunchIntensity = 50.0f;
	mBoundTimer = 0;
	mRandDir = 0;
	mBehavior = POIHANA_BEHAVIOR_NORMAL;
	mCanDrown = false;
	mWaterColumn = NULL;
}

void Poihana::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	MR::initActorCamera(this, rIter, &mCamInfo);
	MR::processInitFunction(this, rIter, false);
	MR::connectToSceneEnemy(this);

	// Setup sensors
	initHitSensor(2);
	MR::addHitSensorPriorBinder(this, "binder", 8, 125.0f, TVec3f(0.0f, 130.0f, 120.0f));
	MR::addHitSensorAtJointEnemy(this, "body", "Body", 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f)); // type 0x1E in SMG1

	// Setup binder
	bool useSmallBinder = false;
	MR::getJMapInfoArg7NoInit(rIter, &useSmallBinder);

	if (useSmallBinder)
		initBinder(100.0f, 100.0f, 0);
	else
		initBinder(150.0f, 150.0f, 0);

	MR::setGroupClipping(this, rIter, 0x20);
	initEffectKeeper(1, NULL, false);
	initSound(4, "Poihana", false, TVec3f(0.0f, 0.0f, 0.0f));

	MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 100.0f, 0.0f));
	mAnimScaleCtrl = new AnimScaleController(NULL);
	mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);

	MR::initShadowVolumeSphere(this, 80.0f);
	MR::addToAttributeGroupSearchTurtle(this);

	mRespawnPos.set<f32>(mTranslation);
	mHomePos.set<f32>(mTranslation);

	// Calculate launch intensity, thanks to Shibbo for helping me on that one
	f32 launchIntensity = 1000.0f;
	MR::getJMapInfoArg0NoInit(rIter, &launchIntensity);
	launchIntensity *= 2.0;

	if (launchIntensity > 0.0f) {
		f32 reciprocal = __frsqrte(launchIntensity);
		f32 factor = reciprocal * launchIntensity;
		launchIntensity = -(factor * reciprocal - 3.0f) * factor * 0.5f;
	}

	mLaunchIntensity = -launchIntensity;

	// Setup behaviors
	MR::getJMapInfoArg3NoInit(rIter, &mBehavior);
	MR::getJMapInfoArg4NoInit(rIter, &mCanDrown);

	if (mCanDrown) {
		mWaterColumn = MR::createModelObjMapObj("エフェクト水柱", "WaterColumn", (Mtx4*)getBaseMtx());
		mWaterColumn->mScale.setAll<f32>(2.0f);
	}

	MR::tryCreateMirrorActor(this, NULL);

	if (mBehavior == POIHANA_BEHAVIOR_SLEEP)
		initNerve(&NrvPoihana::NrvSleep::sInstance, 0);
	else
		initNerve(&NrvPoihana::NrvWait::sInstance, 0);

	makeActorAppeared();
}

void Poihana::initAfterPlacement() {
	TPositionMtx baseMtx;
	MR::makeMtxUpNoSupportPos(&baseMtx, -mGravity, mTranslation);
	MR::setBaseTRMtx(this, baseMtx);
	MR::calcFrontVec(&mFrontVec, this);
	//MR::trySetMoveLimitCollision(this);
}

void Poihana::control() {
	if (!isNerve(&NrvPoihana::NrvHide::sInstance)) {
		mAnimScaleCtrl->updateNerve();
		calcBound();
		controlVelocity();

		TVec3f upVec;
		MR::calcUpVec(&upVec, this);
		TVec3f gravityPos;
		JMAVECScaleAdd((Vec*)&upVec, (Vec*)&mTranslation, (Vec*)&gravityPos, 20.0f);
		MR::calcGravity(this, gravityPos);
		
		if (!tryDrown() && !tryDPDSwoon())
			tryHipDropShock();
	}
}

void Poihana::calcAndSetBaseMtx() {
	TPositionMtx _14;
	MR::calcMtxFromGravityAndZAxis(&_14, this, mGravity, mFrontVec);

	if (isNerveTypeWalkOrWait())
		MR::blendMtx((Mtx4*)getBaseMtx(), (Mtx4*)&_14, 0.3f, (Mtx4*)&_14);

	MR::setBaseTRMtx(this, _14);
	MR::updateBaseScale(this, mAnimScaleCtrl);
}

void Poihana::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
	bool isSensorPlayer = MR::isSensorPlayer(pHit1);

	if (isSensorPlayer || MR::isSensorEnemy(pHit1)) {
		if (isSensorPlayer)
			contactMario(pHit1, pHit2);
		if (MR::sendMsgPush(pHit2, pHit1)) {
			if (isSensorPlayer && !(isNerve(&NrvPoihana::NrvShock::sInstance)
				|| isNerve(&NrvPoihana::NrvSwoonLand::sInstance)
				|| isNerve(&NrvPoihana::NrvSwoon::sInstance)
				|| isNerve(&NrvPoihana::NrvRecover::sInstance)))
				return;

			TVec3f pushVelocity;
			PSVECSubtract((Vec*)&mTranslation, (Vec*)&pHit2->mParentActor->mTranslation, (Vec*)&pushVelocity);
			MR::normalizeOrZero(&pushVelocity);
			
			if (mVelocity.dot(pushVelocity) < 0.0f) {
				f32 dot = pushVelocity.dot(mVelocity) * -1.0f;
				JMAVECScaleAdd((Vec*)&pushVelocity, (Vec*)&mVelocity, (Vec*)&mVelocity, dot);
			}
		}
	}
}

u32 Poihana::receiveMsgPush(HitSensor* pHit1, HitSensor* pHit2) {
	if (!MR::isSensorEnemy(pHit2) && !MR::isSensorMapObj(pHit2))
		return 0;

	TVec3f pushOffset;
	PSVECSubtract((Vec*)&mTranslation, (Vec*)&pHit1->mParentActor->mTranslation, (Vec*)&pushOffset);
	MR::normalizeOrZero(&pushOffset);
	JMAVECScaleAdd((Vec*)&pushOffset, (Vec*)&mTranslation, (Vec*)&mTranslation, 1.5f);

	return 1;
}

u32 Poihana::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgStarPieceAttack(msg))
		return 1;
	else if (MR::isMsgStarPieceReflect(msg)) {
		mAnimScaleCtrl->startHitReaction();
		return 1;
	}

	if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
		if (isNerve(&NrvPoihana::NrvShock::sInstance)
			|| isNerve(&NrvPoihana::NrvSwoonLand::sInstance)
			|| isNerve(&NrvPoihana::NrvSwoon::sInstance)
			|| isNerve(&NrvPoihana::NrvRecover::sInstance)
			|| isNerve(&NrvPoihana::NrvSleepStart::sInstance)
			|| isNerve(&NrvPoihana::NrvSleep::sInstance)
			|| isNerve(&NrvPoihana::NrvGetUp::sInstance))
		{
			startBound();
			MR::startLevelSound(this, "EvPoihanaTrample", -1, -1, -1);

			if (MR::isMsgPlayerHipDrop(msg))
				MR::sendMsgAwayJump(pHit1, pHit2);
			return 1;
		}
		else
			updateTrampleBinderSensor();
	}

	if (MR::isMsgPlayerHitAll(msg) && tryShock()) {
		MR::stopSceneForDefaultHit(3);
		return 1;
	}

	return 0;
}

u32 Poihana::receiveMsgEnemyAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgExplosionAttack(msg) && tryShock())
		return 1;
	return 0;
}

u32 Poihana::receiveOtherMsg(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pHit1) && !isNerve(&NrvPoihana::NrvShootUpCharge::sInstance)) {
		if (!MR::isOnGroundPlayer() && !tryToStartBind(pHit1)) {
			setNerve(&NrvPoihana::NrvShootUp::sInstance);
			return 1;
		}
		else if (MR::isOnGroundPlayer() && isNerve(&NrvPoihana::NrvShootUp::sInstance)) {
			if (MR::isLessStep(this, 30))
				return tryToStartBind(pHit1);
		}
		else if (MR::isNear(pHit1, pHit2, 100.0f))
			setNerve(&NrvPoihana::NrvShootUpCharge::sInstance);
	}
	else if (MR::isMsgUpdateBaseMtx(msg) && mBindedActor)
		updateBindActorMtx();

	return 0;
}

void Poihana::exeWait() {
	if (MR::isFirstStep(this)) {
		MR::startBckNoInterpole(this, "Wait");
		updateBinderSensor();
	}

	if (MR::isNearPlayer(this, 800.0f))
		setNerve(&NrvPoihana::NrvSearch::sInstance);
	else if (MR::isGreaterStep(this, 180))
		setNerve(&NrvPoihana::NrvWalkAround::sInstance);
}

void Poihana::exeWalkAround() {
	if (MR::isFirstStep(this)) {
		if (!MR::isBckPlaying(this, "Walk"))
			MR::startAction(this, "Walk");
		mRandDir = MR::getRandom((s32)-2, (s32)2);
	}

	MR::rotateVecDegree(&mFrontVec, mGravity, mRandDir);
	JMAVECScaleAdd((Vec*)&mFrontVec, (Vec*)&mVelocity, (Vec*)&mVelocity, 0.5f);

	if (isNeedForBackHome())
		setNerve(&NrvPoihana::NrvGoBack::sInstance);
	else if (MR::isNearPlayer(this, 800.0f))
		setNerve(&NrvPoihana::NrvSearch::sInstance);
	else if (MR::isGreaterStep(this, 120)) {
		if (mBehavior == POIHANA_BEHAVIOR_SLEEP)
			setNerve(&NrvPoihana::NrvSleepStart::sInstance);
		else
			setNerve(&NrvPoihana::NrvWait::sInstance);
	}
}

void Poihana::exeSleepStart() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "SleepStart");
		MR::startLevelSound(this, "EvPoihanaSleepStart", -1, -1, -1);
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvPoihana::NrvSleep::sInstance);
}

void Poihana::exeSleep() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Sleep");
		MR::invalidateHitSensor(this, "binder");
	}

	if (isNeedForGetUp())
		setNerve(&NrvPoihana::NrvGetUp::sInstance);
}

void Poihana::exeGetUp() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "GetUp");
		MR::startLevelSound(this, "EvPoihanaWakeup", -1, -1, -1);
		MR::startLevelSound(this, "EmPoihanaWakeup", -1, -1, -1);
	}

	if (MR::isActionEnd(this)) {
		updateBinderSensor();
		setNerve(&NrvPoihana::NrvWait::sInstance);
	}
}

void Poihana::exeSearch() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Search");
		MR::startLevelSound(this, "EvPoihanaFind", -1, -1, -1);
	}

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);

	if (MR::isActionEnd(this))
		setNerve(&NrvPoihana::NrvChasePlayer::sInstance);
}

void Poihana::exeChasePlayer() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Run");
		updateBinderSensor();
	}

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);
	JMAVECScaleAdd((Vec*)&mFrontVec, (Vec*)&mVelocity, (Vec*)&mVelocity, 0.5f);

	if (isNeedForBackHome())
		setNerve(&NrvPoihana::NrvGoBack::sInstance);
}

void Poihana::exeShootUpCharge() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "ThrowStart");

	f32 dot = mGravity.dot(mVelocity);
	mVelocity.scale(dot, mGravity);

	if (MR::isActionEnd(this))
		setNerve(&NrvPoihana::NrvShootUp::sInstance);
}

void Poihana::exeShootUp() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Throw");
		MR::startLevelSound(this, "EvPoihanaShootUp", -1, -1, -1);

		MR::startActorCameraNoTarget(this, mCamInfo, -1);
	}

	f32 dot = mGravity.dot(mVelocity);
	mVelocity.scale(dot, mGravity);

	if (MR::isStep(this, 2)) {
		endBind();
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isStep(this, 30))
		updateBinderSensor();

	if (MR::isActionEnd(this)) {
		if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
			mHomePos.set<f32>(mTranslation);
			setNerve(&NrvPoihana::NrvWalkAround::sInstance);
		}
		else
			setNerve(&NrvPoihana::NrvGoBack::sInstance);
	}
}

void Poihana::endShootUp() {
	endBind();
	updateBinderSensor();

	MR::endActorCamera(this, mCamInfo, true, -1);
}

void Poihana::exeGoBack() {
	if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Walk"))
		MR::startAction(this, "Walk");

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, mHomePos, 2.0f);
	JMAVECScaleAdd((Vec*)&mFrontVec, (Vec*)&mVelocity, (Vec*)&mVelocity, 0.5f);

	if (MR::isNearPlayer(this, 800.0f) && MR::isGreaterStep(this, 120))
		setNerve(&NrvPoihana::NrvSearch::sInstance);
	else if (MR::isNear(this, mHomePos, 100.0f))
		setNerve(&NrvPoihana::NrvWait::sInstance);
}

void Poihana::exeShock() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "PunchDamage");
		MR::startBlowHitSound(this);
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isOnGround(this) && MR::isGreaterStep(this, 12))
		setNerve(&NrvPoihana::NrvSwoon::sInstance);
}

void Poihana::exeSwoonLand() {
	if (MR::isFirstStep(this)) {
		MR::startBckNoInterpole(this, "SwoonLand");
		MR::startLevelSound(this, "EvPoihanaSwoon", -1, -1, -1);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvPoihana::NrvSwoon::sInstance);
}

void Poihana::exeSwoon() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "Swoon");

	MR::startLevelSound(this, "EmLvSwoonS", -1, -1, -1);

	if (MR::isStep(this, 110))
		setNerve(&NrvPoihana::NrvRecover::sInstance);
}

void Poihana::exeRecover() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Recover");
		MR::startLevelSound(this, "EmPoihanaRecover", -1, -1, -1);
		MR::startLevelSound(this, "EvPoihanaRecover", -1, -1, -1);
		mScale.setAll<f32>(1.0f);
	}

	if (MR::isActionEnd(this)) {
		updateBinderSensor();
		setNerve(&NrvPoihana::NrvWait::sInstance);
	}
}

void Poihana::exeShake() {
	s32 step = getNerveStep();
	f32 _f31 = 0.2f - 0.01f * step;
	f32 scale = sin(36.0f * step * 0.01745329251f) * _f31 + 1.0f;
	mScale.setAll<f32>(scale);

	if (MR::isStep(this, 20)) {
		mScale.setAll<f32>(1.0f);
		setNerve(&NrvPoihana::NrvSearch::sInstance);
	}
}

void Poihana::exeDrown() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Drown");
		MR::invalidateClipping(this);
		MR::invalidateHitSensors(this);
		MR::startLevelSound(this, "EmFallIntoWaterS", -1, -1, -1);
		MR::tryStartAllAnim(mWaterColumn, "Splash");
		mWaterColumn->appear();
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvPoihana::NrvHide::sInstance);
}

void Poihana::exeHide() {
	if (MR::isFirstStep(this)) {
		MR::startLevelSound(this, "EmExplodeSWater", -1, -1, -1);
		MR::emitEffect(this, "DeathWater");
		MR::hideModel(this);
	}

	if (MR::isStep(this, 150))
		setNerve(&NrvPoihana::NrvAppear::sInstance);
}

void Poihana::exeAppear() {
	if (MR::isFirstStep(this)) {
		MR::resetPosition(this, mRespawnPos);
		MR::emitEffect(this, "Appear");
	}

	if (MR::isLessStep(this, 60))
		MR::startLevelSound(this, "EmLvPoihanaReviveEffect", -1, -1, -1);

	if (MR::isStep(this, 60)) {
		MR::showModel(this);
		MR::startAction(this, "Appear");
		MR::startLevelSound(this, "EmPoihanaReviveAppear", -1, -1, -1);
	}

	if (MR::isActionEnd(this) && MR::isGreaterStep(this, 60)) {
		MR::validateClipping(this);
		MR::validateHitSensors(this);
		updateBinderSensor();
		setNerve(&NrvPoihana::NrvWait::sInstance);
	}
}

void Poihana::exeDPDSwoon() {
	if (MR::isFirstStep(this)) {
		MR::invalidateHitSensor(this, "binder");
		mAnimScaleCtrl->startDpdHitVibration();
	}

	MR::startDPDFreezeLevelSound(this);
	MR::updateActorStateAndNextNerve(this, (ActorStateBaseInterface*)mBindStarPointer, &NrvPoihana::NrvWait::sInstance);
}

void Poihana::endDPDSwoon() {
	updateBinderSensor();
	MR::tryDeleteEffect(this, "PointerTouch");
	mAnimScaleCtrl->startAnim();
}

void Poihana::updateBinderSensor() {
	HitSensor* senBinder = getSensor("binder");
	senBinder->_4.set<f32>(0.0f, 130.0, 120.0f);
	senBinder->_10 = 125.0f;
	senBinder->validate();
}

void Poihana::updateTrampleBinderSensor() {
	HitSensor* senBinder = getSensor("binder");
	senBinder->_4.set<f32>(0.0f, 150.0, 0.0f);
	senBinder->_10 = 200.0f;
	senBinder->validate();
}

bool Poihana::tryToStartBind(HitSensor* pHit1) {
	if (mBindedActor == NULL && !MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
		MR::tryRumblePadMiddle(this, 0);
		mBindedActor = pHit1->mParentActor;
		MR::startBckPlayer("Rise", NULL);
		MR::invalidateClipping(this);
		return true;
	}

	return false;
}

void Poihana::updateBindActorMtx() {
	TPositionMtx binderMtx;
	MR::makeMtxTR((Mtx4*)&binderMtx, mBindedActor);
	MR::setBaseTRMtx(mBindedActor, binderMtx);
}

void Poihana::endBind() {
	MR::validateClipping(this);
	
	if (MR::isPlayerInRush()) {
		TVec3f jumpVec;
		jumpVec.scale(mLaunchIntensity, mGravity);
		MR::endBindAndPlayerJump(this, jumpVec, 0);
		MR::startLevelSound(this, "PmHeliJump", -1, -1, -1);
	}

	mBindedActor = NULL;
}

void Poihana::startBound() {
	mBoundTimer = 0;
	mScale.setAll<f32>(1.0f);
}

/*
* This function calculates Poihana's scale for 40 frames after being trampled. This is
* used to simulate the "vibrating" visual effect. This is not 1:1 the same as in SMG1,
* but it looks VERY similar and appears to be even more efficient compared to SMG1's
* unusualy complicated calculations.
*/
void Poihana::calcBound() {
	if (mBoundTimer != -1) {
		f32 scale = 1.0f;

		if (mBoundTimer < 40) {
			scale = 0.05f * sin(0.393f * mBoundTimer) + 1.0f;
			mBoundTimer++;
		}
		else
			mBoundTimer = -1;

		mScale.setAll<f32>(scale);
	}
}

void Poihana::contactMario(HitSensor* pHit1, HitSensor* pHit2) {
	if (!isNerveTypeWalkOrWait() && isNerve(&NrvPoihana::NrvSleep::sInstance))
		setNerve(&NrvPoihana::NrvGetUp::sInstance);

	if (isBackAttack(pHit2) && MR::sendMsgEnemyAttackFlipWeak(pHit2, pHit1))
		setNerve(&NrvPoihana::NrvShake::sInstance);
}

void Poihana::controlVelocity() {
	TVec3f gravity;
	if (MR::isBindedGround(this))
		gravity.set<f32>(-*MR::getGroundNormal(this));
	else
		gravity.set<f32>(mGravity);

	TVec3f frontVec(mFrontVec);
	MR::turnVecToPlane(&mFrontVec, frontVec, gravity);

	if (MR::isBindedGround(this)) {
		f32 dot = mFrontVec.dot(mVelocity) * -1.0f;

		TVec3f addVel;
		JMAVECScaleAdd((Vec*)&mFrontVec, (Vec*)&mVelocity, (Vec*)&addVel, dot);
		addVel.scale(0.8f);
		mVelocity.scale(mFrontVec.dot(mVelocity), mFrontVec);
		mVelocity.add(addVel);

		if (mVelocity.dot(gravity) > 0.0f) {
			dot = gravity.dot(mVelocity) * -1.0f;
			JMAVECScaleAdd((Vec*)&gravity, (Vec*)&mVelocity, (Vec*)&mVelocity, dot);
		}

		mVelocity.scale(0.95f);
	}

	JMAVECScaleAdd((Vec*)&gravity, (Vec*)&mVelocity, (Vec*)&mVelocity, 2.0f);

	if (!isNerve(&NrvPoihana::NrvShock::sInstance)) {
		f32 magVel = isNerve(&NrvPoihana::NrvChasePlayer::sInstance) ? 10.0f : 5.0f;

		if (PSVECMag((Vec*)&mVelocity) > magVel) {
			f32 sqx = mVelocity.x * mVelocity.x;
			f32 sqy = mVelocity.y * mVelocity.y;
			f32 sqz = mVelocity.z * mVelocity.z;
			f32 sqmag = sqx + sqy + sqz;

			if (sqmag > 0.0000038146973f) {
				f32 invsqrt = JGeometry::TUtil<f32>::inv_sqrt(sqmag);
				mVelocity.scale(invsqrt);
			}
		}

		if (MR::isNearZero(mVelocity, 0.001f))
			mVelocity.zero();
	}
}

bool Poihana::tryDrown() {
	if (!mCanDrown)
		return false;
	if (isNerve(&NrvPoihana::NrvAppear::sInstance) || isNerve(&NrvPoihana::NrvDrown::sInstance))
		return false;
	if (!MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f)))
		return false;

	setNerve(&NrvPoihana::NrvDrown::sInstance);
	return true;
}

bool Poihana::tryDPDSwoon() {
	if (isNerveTypeWalkOrWait() && mBindStarPointer->tryStartPointBind()) {
		setNerve(&NrvPoihana::NrvDPDSwoon::sInstance);
		return true;
	}

	return false;
}

bool Poihana::tryShock() {
	if (isNerve(&NrvPoihana::NrvDrown::sInstance)
		|| isNerve(&NrvPoihana::NrvHide::sInstance)
		|| isNerve(&NrvPoihana::NrvAppear::sInstance)
		|| isNerve(&NrvPoihana::NrvShock::sInstance)
		|| isNerve(&NrvPoihana::NrvSwoonLand::sInstance))
		return false;

	mVelocity.scale(-30.0f, mGravity);
	setNerve(&NrvPoihana::NrvShock::sInstance);
	return true;
}

bool Poihana::tryHipDropShock() {
	if (!MR::isNearPlayerAnyTime(this, 500.0f))
		return false;
	if (MR::isPlayerHipDropLand())
		return tryShock();
	return false;
}

bool Poihana::isNerveTypeWalkOrWait() const {
	return (isNerve(&NrvPoihana::NrvWait::sInstance)
		|| isNerve(&NrvPoihana::NrvSearch::sInstance)
		|| isNerve(&NrvPoihana::NrvWalkAround::sInstance)
		|| isNerve(&NrvPoihana::NrvChasePlayer::sInstance)
		|| isNerve(&NrvPoihana::NrvGoBack::sInstance));
}

bool Poihana::isNeedForBackHome() const {
	if (isNerve(&NrvPoihana::NrvWalkAround::sInstance))
		return MR::isNear(this, mHomePos, 350.0f) == 0;
	if (!isNerve(&NrvPoihana::NrvChasePlayer::sInstance))
		return false;
	if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME)
		return MR::isNearPlayer(this, 1100.0f) == 0;
	if (!MR::isNear(this, mHomePos, 2000.0f) || !MR::isNearPlayer(this, 1110.0f))
		return true;
	return false;
}

bool Poihana::isNeedForGetUp() const {
	if (MR::isNearPlayer(this, 500.0f)) {
		f32 mag = PSVECMag((Vec*)MR::getPlayerVelocity());

		if (mag < 10.0f && !MR::isPlayerSwingAction())
			return false;
		return true;
	}

	return false;
}

bool Poihana::isBackAttack(HitSensor* pHit) const {
	TVec3f frontVec;
	MR::calcFrontVec(&frontVec, this);
	frontVec.negate();

	TVec3f sensorRelative;
	PSVECSubtract((Vec*)&pHit->_4, (Vec*)&mTranslation, (Vec*)&sensorRelative);
	return sensorRelative.dot(frontVec) > 0.0f;
}

namespace NrvPoihana {
	void NrvWait::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvWalkAround::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeWalkAround();
	}

	void NrvSleepStart::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeSleepStart();
	}

	void NrvSleep::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeSleep();
	}

	void NrvGetUp::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeGetUp();
	}

	void NrvSearch::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeSearch();
	}

	void NrvChasePlayer::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeChasePlayer();
	}

	void NrvShootUpCharge::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeShootUpCharge();
	}

	void NrvShootUp::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeShootUp();
	}

	void NrvShootUp::executeOnEnd(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->endShootUp();
	}

	void NrvGoBack::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeGoBack();
	}

	void NrvShock::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeShock();
	}

	void NrvSwoon::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeSwoon();
	}

	void NrvSwoonLand::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeSwoonLand();
	}

	void NrvRecover::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeRecover();
	}

	void NrvShake::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeShake();
	}

	void NrvDrown::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeDrown();
	}

	void NrvHide::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeHide();
	}

	void NrvAppear::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeAppear();
	}

	void NrvDPDSwoon::execute(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->exeDPDSwoon();
	}

	void NrvDPDSwoon::executeOnEnd(Spine* spine) const {
		Poihana* pActor = (Poihana*)spine->mExecutor;
		pActor->endDPDSwoon();
	}

	NrvWait(NrvWait::sInstance);
	NrvWalkAround(NrvWalkAround::sInstance);
	NrvSleepStart(NrvSleepStart::sInstance);
	NrvSleep(NrvSleep::sInstance);
	NrvGetUp(NrvGetUp::sInstance);
	NrvSearch(NrvSearch::sInstance);
	NrvChasePlayer(NrvChasePlayer::sInstance);
	NrvShootUpCharge(NrvShootUpCharge::sInstance);
	NrvShootUp(NrvShootUp::sInstance);
	NrvGoBack(NrvGoBack::sInstance);
	NrvShock(NrvShock::sInstance);
	NrvSwoon(NrvSwoon::sInstance);
	NrvSwoonLand(NrvSwoonLand::sInstance);
	NrvRecover(NrvRecover::sInstance);
	NrvShake(NrvShake::sInstance);
	NrvDrown(NrvDrown::sInstance);
	NrvHide(NrvHide::sInstance);
	NrvAppear(NrvAppear::sInstance);
	NrvDPDSwoon(NrvDPDSwoon::sInstance);
}
