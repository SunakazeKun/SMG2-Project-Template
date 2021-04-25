#include "spack/Actor/JumpGuarder.h"
#include "spack/SPackUtil.h"
#include "Actor/Animation/BckControl.h"
#include "Actor/Enemy/BegomanBaby.h"
#include "Util/ActorMovementUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"
#include "Util/JointUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/MathUtil.h"
#include "Util/MtxUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SoundUtil.h"
#include "mtx.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

JumpGuarder::JumpGuarder(const char* pName) : JumpEmitter(pName) {
	// Instantiate nerves
	for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
		(*f)();

	mBabys = NULL;
	mNumBabys = 4;
	mStaggerDelay = 0;
	mNumPendingBabys = 0;
	mLaunchVelocity = 16.0f;
}

void JumpGuarder::init(const JMapInfoIter& rIter) {
	initModelManagerWithAnm("JumpGuarder", NULL);
	mHead = MR::createPartsModelNoSilhouettedMapObj(this, "ジャンプガーダー頭", "JumpGuarderHead", (Mtx4*)&_94);
	MR::initLightCtrl(mHead);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemy(this);
	MR::initLightCtrl(this);

	initHitSensor(2);

	JGeometry::TVec3<f32>* vec = new JGeometry::TVec3<f32>(0.0f, -100.0f, 0.0f);
	Mtx4* jointMtx = (Mtx4*)MR::getJointMtx(mHead, "SpringJoint3");
	MR::addHitSensorMtx(this, "Jump", 0x23, 8, 145.0f, jointMtx, *vec);

	vec = new JGeometry::TVec3<f32>(0.0f, 35.0f, 0.0f);
	jointMtx = (Mtx4*)MR::getJointMtx(this, "Body");
	MR::addHitSensorMtxEnemy(this, "Body", 8, 145.0f, jointMtx, *vec);

	getSensor("Body")->setType(0x21);
	MR::validateHitSensor(this, "Body");
	MR::invalidateHitSensor(this, "Jump");

	MR::initShadowVolumeSphere(this, 140.0f);
	initSound(8, "JumpGuarder", false, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	MR::invalidateClipping(this);
	initNerve(&NrvJumpGuarder::NrvHide::sInstance, 0);

	MR::startBckWithInterpole(this, "Down", 0);
	MR::setBckFrame(this, 0.0f);
	MR::calcAnimDirect(this);
	mJointMtx = (Mtx*)MR::getJointMtx(this, "Body")->val;
	MR::useStageSwitchReadA(this, rIter);
	MR::useStageSwitchReadB(this, rIter);
	JumpEmitter::init(rIter);
	MR::joinToGroupArray(this, rIter, 0, 0x20);

	makeActorAppeared();

	// Read Obj_arg entries
	MR::getJMapInfoArg1NoInit(rIter, &mLaunchVelocity);
	MR::getJMapInfoArg2NoInit(rIter, &mNumBabys);
	SPack::clamp(1, 4, &mNumBabys);

	// Create BegomanBabys
	mBabys = new BegomanBaby*[mNumBabys];

	for (s32 i = 0; i < mNumBabys; i++) {
		BegomanBaby* baby = new BegomanBaby("ベビーベーゴマン");
		baby->mHost = this;
		baby->mTranslation.set<f32>(mTranslation);
		baby->initWithoutIter();
		baby->makeActorDead();
		mBabys[i] = baby;
	}
}

void JumpGuarder::control() {
	Mtx mtxTRS;
	MR::makeMtxTRS(mtxTRS, JGeometry::TVec3<f32>(0.0f, 44.0f, 0.0f), mHead->mRotation, mHead->mScale);
	PSMTXCopy((Mtx4*)mJointMtx, _94);
	PSMTXConcat(_94, mtxTRS, _94);

	// Decrement stagger delay and ensure the value does not fall below 0
	mStaggerDelay = (mStaggerDelay - 1) & (((u32)(mStaggerDelay - 1) >> 31) - 1);

	updateEventCamera();
	JumpEmitter::control();
}

void JumpGuarder::attackSensor(HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isSensorEnemy(pHit2) && isHit(pHit2->mParentActor)) {
		MR::sendMsgEnemyAttack(pHit2, pHit1);

		if (isNerve(&NrvJumpGuarder::NrvWait::sInstance) && !MR::isOnGround(pHit2->mParentActor)) {
			MR::startBck(this, "Hit");
			MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
		}
	}
	else if (MR::isSensorPlayer(pHit2)) {
		MR::sendMsgPush(pHit2, pHit1);

		if (isNerve(&NrvJumpGuarder::NrvWait::sInstance) || isNerve(&NrvJumpGuarder::NrvHopWait::sInstance)) {
			if (MR::isPlayerStaggering() && mStaggerDelay == 0) {
				mStaggerDelay = 60;
				MR::startBck(this, "Hit");
				MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
			}
		}
	}
}

u32 JumpGuarder::receiveMsgPlayerAttack(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (MR::isMsgPlayerTrample(msg)) {
		if (pHit2->isType(0x23)) {
			JGeometry::TVec3<f32> upVec;
			MR::calcUpVec(&upVec, this);
			MR::setPlayerJumpVec(upVec);

			startEventCamera();
			setNerve(&NrvJumpGuarder::NrvHopJump::sInstance);
		}
		else
			setNerve(&NrvJumpGuarder::NrvHopStart::sInstance);
		return 1;
	}
	else if (MR::isMsgPlayerHipDrop(msg)) {
		MR::forceJumpPlayer(JGeometry::TVec3<f32>(-mGravity.x, -mGravity.y, -mGravity.z));
		return 1;
	}
	else if (MR::isMsgPlayerSpinAttack(msg)) {
		if (!isNerve(&NrvJumpGuarder::NrvHopStart::sInstance))
			setNerve(&NrvJumpGuarder::NrvHopStart::sInstance);
		return 1;
	}
	else
		return MR::isMsgStarPieceReflect(msg);
	return 0;
}

u32 JumpGuarder::receiveOtherMsg(u32 msg, HitSensor* pHit1, HitSensor* pHit2) {
	if (msg == ACTMES_GROUP_ATTACK) {
		MR::invalidateClipping(this);
		setNerve(&NrvJumpGuarder::NrvUp::sInstance);
		return 1;
	}
	else if (msg == ACTMES_GROUP_HIDE) {
		setNerve(&NrvJumpGuarder::NrvDown::sInstance);
		return 1;
	}
	return 0;
}

void JumpGuarder::exeHide() {
	updateRotate();

	if (MR::isFirstStep(this)) {
		MR::startBck(mHead, "Wait");
		MR::startBrk(mHead, "Green");
		MR::validateClipping(this);
		MR::setShadowVolumeSphereRadius(this, NULL, 110.0f);
	}
	else if (MR::enableGroupAttack(this, 2000.0f, 500.0f))
		MR::sendMsgToGroupMember(ACTMES_GROUP_ATTACK, this, getSensor("Body"), "Body");
}

void JumpGuarder::exeUp() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Up");
		MR::startLevelSound(this, "EmJguarderAppear", -1, -1, -1);
	}

	f32 bckMaxFrame = MR::getBckCtrl(this)->_8;
	f32 bckFrame = MR::getBckFrame(this);
	f32 radius = 110.0f + (bckFrame / bckMaxFrame) * 30.0f;
	MR::setShadowVolumeSphereRadius(this, NULL, radius);

	if (MR::isActionEnd(this)) {
		MR::setShadowVolumeSphereRadius(this, NULL, 140.0f);
		setNerve(&NrvJumpGuarder::NrvWait::sInstance);
	}
}

void JumpGuarder::exeWait() {
	updateRotate();

	if (!MR::enableGroupAttack(this, 2200.0f, 500.0f))
		MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");
	else if (enableAttack())
		setNerve(&NrvJumpGuarder::NrvPreOpen::sInstance);
}

void JumpGuarder::exeDown() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Down");
		MR::startLevelSound(this, "EmJguarderHide", -1, -1, -1);
	}

	f32 bckMaxFrame = MR::getBckCtrl(this)->_8;
	f32 bckFrame = MR::getBckFrame(this);
	f32 radius = 110.0f + (1.0f - (bckFrame / bckMaxFrame)) * 30.0f;
	MR::setShadowVolumeSphereRadius(this, NULL, radius);

	if (MR::isActionEnd(this))
		setNerve(&NrvJumpGuarder::NrvHide::sInstance);
}

void JumpGuarder::exeHopStart() {
	if (MR::isFirstStep(this)) {
		MR::startBck(mHead, "HopStart");
		MR::startBrk(mHead, "OnAndOff");
		MR::startBck(this, "Damage");
		MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
		getSensor("Body")->_10 = 120.0f;
	}

	MR::startLevelSound(this, "EmLvJguarderShake", -1, -1, -1);

	// If shaken during the launching phase, kill all pending BegomanBabys
	if (MR::isStep(this, 10)) {
		for (u32 i = 0; i < mNumPendingBabys; i++)
			mPendingBabys[i]->makeActorDead();

		mNumPendingBabys = 0;
	}

	if (MR::isActionEnd(mHead))
		setNerve(&NrvJumpGuarder::NrvHopWait::sInstance);
}

void JumpGuarder::exeHopWait() {
	if (MR::isFirstStep(this)) {
		MR::stopBck(this);
		MR::startBck(mHead, "HopWait");
		MR::validateHitSensor(this, "Jump");
	}

	MR::startLevelSound(this, "EmLvJguarderShake", -1, -1, -1);

	if (MR::isStep(this, 300))
		setNerve(&NrvJumpGuarder::NrvHopEnd::sInstance);
}

void JumpGuarder::exeHopJump() {
	if (MR::isFirstStep(this)) {
		MR::startBck(mHead, "HopJump");
		MR::startLevelSound(this, "EmJguarderTrample", -1, -1, -1);
	}

	if (MR::isActionEnd(mHead))
		setNerve(&NrvJumpGuarder::NrvHopWait::sInstance);
}

void JumpGuarder::exeHopEnd() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "HopEnd");
		MR::startBck(mHead, "HopEnd");
		MR::startBrk(mHead, "Green");
		MR::startLevelSound(this, "EmJguarderCloseSpring", -1, -1, -1);
	}

	if (MR::isActionEnd(this) && MR::isActionEnd(mHead)) {
		getSensor("Body")->_10 = 145.0f;
		MR::invalidateHitSensor(this, "Jump");
		setNerve(&NrvJumpGuarder::NrvWait::sInstance);
	}
}

void JumpGuarder::exePreOpen() {
	updateRotate();

	if (!MR::enableGroupAttack(this, 2200.0f, 500.0f))
		MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");

	if (MR::isFirstStep(this))
		setNerve(&NrvJumpGuarder::NrvOpen::sInstance);
}

void JumpGuarder::exeOpen() {
	if (MR::isLessStep(this, 70))
		updateRotate();

	// Collect and prepare new BegomanBabys to be launched
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Open");
		MR::startLevelSound(this, "EmJguarderShutterOpen", -1, -1, -1);

		// Collect dead BegomanBabys to "revive" them
		mNumPendingBabys = 0;
		for (s32 i = 0; i < mNumBabys; i++) {
			if (MR::isDead(mBabys[i]))
				mPendingBabys[mNumPendingBabys++] = mBabys[i];
		}

		Mtx* headBaseMtx = mHead->getBaseMtx();
		JGeometry::TVec3<f32> ydir;
		JGeometry::TVec3<f32> zdir;
		MR::extractMtxYDir(*headBaseMtx, &ydir);
		MR::extractMtxZDir(*headBaseMtx, &zdir);

		for (s32 i = 0; i < mNumPendingBabys; i++) {
			BegomanBaby* baby = mPendingBabys[i];

			MR::rotateVecDegree(&zdir, ydir, 360.0f / mNumPendingBabys);
			baby->mTranslation.set<f32>(mTranslation + zdir * 64.0f);
			baby->mVelocity.set<f32>(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

			baby->appearFromGuarder();
		}
	}
	// Keep BegomanBabys inside until launch
	else if (MR::isLessStep(this, 70)) {
		Mtx* headBaseMtx = mHead->getBaseMtx();
		JGeometry::TVec3<f32> ydir;
		JGeometry::TVec3<f32> zdir;
		MR::extractMtxYDir(*headBaseMtx, &ydir);
		MR::extractMtxZDir(*headBaseMtx, &zdir);

		for (s32 i = 0; i < mNumPendingBabys; i++) {
			BegomanBaby* baby = mPendingBabys[i];

			MR::rotateVecDegree(&zdir, ydir, 360.0f / mNumPendingBabys);
			baby->mTranslation.set<f32>(mTranslation + zdir * 64.0f);
			baby->mVelocity.set<f32>(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		}
	}
	// Finally, launch BegomanBabys
	else if (MR::isStep(this, 70)) {
		Mtx* headBaseMtx = mHead->getBaseMtx();
		JGeometry::TVec3<f32> ydir;
		JGeometry::TVec3<f32> zdir;
		MR::extractMtxYDir(*headBaseMtx, &ydir);
		MR::extractMtxZDir(*headBaseMtx, &zdir);

		for (s32 i = 0; i < mNumPendingBabys; i++) {
			BegomanBaby* baby = mPendingBabys[i];

			MR::rotateVecDegree(&zdir, ydir, 360.0f / mNumPendingBabys);
			baby->mVelocity.set<f32>(zdir * mLaunchVelocity);
		}

		MR::startLevelSound(this, "EmJguarderLaunchBaby", -1, -1, -1);
	}
	// Invalidate pending BegomanBabys
	else if (MR::isStep(this, 74))
		mNumPendingBabys = 0;

	if (MR::isActionEnd(this))
		setNerve(&NrvJumpGuarder::NrvClose::sInstance);
}

void JumpGuarder::exeClose() {
	if (MR::isFirstStep(this))
		MR::startBck(this, "Close");

	MR::startLevelSound(this, "EmLvJguarderShutterClose", -1, -1, -1);

	if (MR::isActionEnd(this))
		setNerve(&NrvJumpGuarder::NrvInter::sInstance);
}

void JumpGuarder::exeInter() {
	updateRotate();

	if (!MR::enableGroupAttack(this, 2200.0f, 500.0f))
		MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");

	setNerve(&NrvJumpGuarder::NrvWait::sInstance);
}

bool JumpGuarder::enableAttack() {
	for (s32 i = 0; i < mNumBabys; i++) {
		if (MR::isDead(mBabys[i]))
			return getNerveStep() % 360 == 0;
	}
	return false;
}

/*
* We have to check if the actor that hits this JumpGuarder's sensors is none of the
* pending children inside of it. This is necessary to prevent these BegomanBabys from
* causing EmJguarderHit "spam".
*/
bool JumpGuarder::isHit(const LiveActor* pActor) const {
	for (u32 i = 0; i < mNumPendingBabys; i++) {
		if (mPendingBabys[i] == pActor)
			return false;
	}
	return true;
}

void BegomanBaby::appearFromGuarder() {
	BegomanBase::appear();
	setNerve(&NrvBegomanBaby::HostTypeNrvLaunchFromGuarder::sInstance);
}

namespace NrvJumpGuarder {
	void NrvHide::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeHide();
	}

	void NrvUp::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeUp();
	}

	void NrvWait::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvDown::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeDown();
	}

	void NrvHopStart::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeHopStart();
	}

	void NrvHopWait::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeHopWait();
	}

	void NrvHopJump::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeHopJump();
	}

	void NrvHopEnd::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeHopEnd();
	}

	void NrvPreOpen::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exePreOpen();
	}

	void NrvOpen::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeOpen();
	}

	void NrvClose::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeClose();
	}

	void NrvInter::execute(Spine* spine) const {
		JumpGuarder* pActor = (JumpGuarder*)spine->mExecutor;
		pActor->exeInter();
	}

	NrvHide(NrvHide::sInstance);
	NrvUp(NrvUp::sInstance);
	NrvWait(NrvWait::sInstance);
	NrvDown(NrvDown::sInstance);
	NrvHopStart(NrvHopStart::sInstance);
	NrvHopWait(NrvHopWait::sInstance);
	NrvHopJump(NrvHopJump::sInstance);
	NrvHopEnd(NrvHopEnd::sInstance);
	NrvPreOpen(NrvPreOpen::sInstance);
	NrvOpen(NrvOpen::sInstance);
	NrvClose(NrvClose::sInstance);
	NrvInter(NrvInter::sInstance);
}
