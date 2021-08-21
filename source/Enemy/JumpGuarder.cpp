#include "spack/Enemy/JumpGuarder.h"
#include "spack/Util.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: JumpGuarder
* Parameters:
* - Obj_arg1, float, 16.0f: Launch velocity
* - Obj_arg2, long, 4: Number of Topmini
* - SW_AWAKE
* - SW_A, use, read: Make active
* - SW_B, use, read: Make passive
* 
* JumpGuarder is a green, stationary Spring Topman that emits several Topmini when approached by
* the player. The head functions as a trampoline that can be used by the player to gain height.
*/

JumpGuarder::JumpGuarder(const char *pName) : JumpEmitter(pName) {
	mBabys = NULL;
	mNumBabys = 4;
	mStaggerDelay = 0;
	mNumPendingBabys = 0;
	mLaunchVelocity = 16.0f;
}

void JumpGuarder::init(const JMapInfoIter &rIter) {
	MR::processInitFunction(this, "JumpGuarder", false);
	mHead = MR::createPartsModelNoSilhouettedMapObj(this, "ジャンプガーダー頭", "JumpGuarderHead", (MtxPtr)&mHeadMtx);
	MR::initLightCtrl(mHead);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemy(this);
	MR::initLightCtrl(this);

	// Initialize sensors
	initHitSensor(2);

	MtxPtr jointMtx = (MtxPtr)MR::getJointMtx(mHead, "SpringJoint3");
	MR::addHitSensorMtx(this, "Jump", ATYPE_PLAYER_AUTO_JUMP, 8, 145.0f, jointMtx, TVec3f(0.0f, -100.0f, 0.0f));
	jointMtx = (MtxPtr)MR::getJointMtx(this, "Body");
	MR::addHitSensorMtxEnemy(this, "Body", 8, 145.0f, jointMtx, TVec3f(0.0f, 35.0f, 0.0f));

	getSensor("Body")->setType(ATYPE_BEGOMAN);
	MR::validateHitSensor(this, "Body");
	MR::invalidateHitSensor(this, "Jump");

	MR::initShadowVolumeSphere(this, 140.0f);
	initSound(8, "JumpGuarder", false, TVec3f(0.0f, 0.0f, 0.0f));
	MR::invalidateClipping(this);
	initNerve(&NrvJumpGuarder::NrvHide::sInstance, 0);

	MR::startBckWithInterpole(this, "Down", 0);
	MR::setBckFrame(this, 0.0f);
	MR::calcAnimDirect(this);
	mJointMtx = (Mtx*)MR::getJointMtx(this, "Body");

	MR::useStageSwitchAwake(this, rIter);
	MR::useStageSwitchReadA(this, rIter);
	MR::useStageSwitchReadB(this, rIter);

	JumpEmitter::init(rIter);
	MR::joinToGroupArray(this, rIter, 0, 0x20);

	// Initialize SupportTico target
	MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 50.0f, 0.0f));

	// Read Obj_arg entries
	MR::getJMapInfoArg1NoInit(rIter, &mLaunchVelocity);
	MR::getJMapInfoArg2NoInit(rIter, &mNumBabys);
	SPack::clamp<s32>(1, 4, &mNumBabys);

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

	makeActorAppeared();
}

void JumpGuarder::control() {
	MR::attachSupportTicoToTarget(this);

	Mtx mtxTRS;
	MR::makeMtxTRS(mtxTRS, TVec3f(0.0f, 44.0f, 0.0f), mHead->mRotation, mHead->mScale);
	PSMTXCopy(*mJointMtx, mHeadMtx);
	PSMTXConcat(mHeadMtx, mtxTRS, mHeadMtx);

	// Decrement stagger delay and ensure the value does not fall below 0
	mStaggerDelay = (mStaggerDelay - 1) & (((u32)(mStaggerDelay - 1) >> 31) - 1);

	updateEventCamera();
	JumpEmitter::control();
}

void JumpGuarder::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isSensorEnemy(pReceiver) && isHit(pReceiver->mParentActor)) {
		MR::sendMsgEnemyAttack(pReceiver, pSender);

		if (isNerve(&NrvJumpGuarder::NrvWait::sInstance) && !MR::isOnGround(pReceiver->mParentActor)) {
			MR::startAction(this, "Hit");
			MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
		}
	}
	else if (MR::isSensorPlayer(pReceiver)) {
		MR::sendMsgPush(pReceiver, pSender);

		if (isNerve(&NrvJumpGuarder::NrvWait::sInstance) || isNerve(&NrvJumpGuarder::NrvHopWait::sInstance)) {
			if (MR::isPlayerStaggering() && mStaggerDelay == 0) {
				mStaggerDelay = 60;
				MR::startAction(this, "Hit");
				MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
			}
		}
	}
}

bool JumpGuarder::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgPlayerTrample(msg)) {
		if (pReceiver->isType(ATYPE_PLAYER_AUTO_JUMP)) {
			TVec3f upVec;
			MR::calcUpVec(&upVec, this);
			MR::setPlayerJumpVec(upVec);

			startEventCamera();
			setNerve(&NrvJumpGuarder::NrvHopJump::sInstance);
		}
		else
			setNerve(&NrvJumpGuarder::NrvHopStart::sInstance);
		return true;
	}
	else if (MR::isMsgPlayerHipDrop(msg)) {
		MR::forceJumpPlayer(TVec3f(-mGravity.x, -mGravity.y, -mGravity.z));
		return true;
	}
	else if (MR::isMsgPlayerSpinAttackOrSupportTico(msg)) {
		if (!isNerve(&NrvJumpGuarder::NrvHopStart::sInstance)) {
			setNerve(&NrvJumpGuarder::NrvHopStart::sInstance);
			return true;
		}
	}
	else
		return MR::isMsgStarPieceReflect(msg);
	return false;
}

bool JumpGuarder::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (msg == ACTMES_GROUP_ATTACK) {
		MR::invalidateClipping(this);
		setNerve(&NrvJumpGuarder::NrvUp::sInstance);
		return true;
	}
	else if (msg == ACTMES_GROUP_HIDE) {
		setNerve(&NrvJumpGuarder::NrvDown::sInstance);
		return true;
	}
	return false;
}

void JumpGuarder::exeHide() {
	updateRotate();

	if (MR::isFirstStep(this)) {
		MR::startAction(mHead, "Wait");
		MR::startBrk(mHead, "Green");
		MR::validateClipping(this);
		MR::setShadowVolumeSphereRadius(this, NULL, 110.0f);
	}
	else if (MR::enableGroupAttack(this, 2000.0f, 500.0f))
		MR::sendMsgToGroupMember(ACTMES_GROUP_ATTACK, this, getSensor("Body"), "Body");
}

void JumpGuarder::exeUp() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "Up");
		MR::startLevelSound(this, "EmJguarderAppear", -1, -1, -1);
	}

	f32 bckMaxFrame = MR::getBckFrameMax(this);
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
		MR::startAction(this, "Down");
		MR::startLevelSound(this, "EmJguarderHide", -1, -1, -1);
	}

	f32 bckMaxFrame = MR::getBckFrameMax(this);
	f32 bckFrame = MR::getBckFrame(this);
	f32 radius = 110.0f + (1.0f - (bckFrame / bckMaxFrame)) * 30.0f;
	MR::setShadowVolumeSphereRadius(this, NULL, radius);

	if (MR::isActionEnd(this))
		setNerve(&NrvJumpGuarder::NrvHide::sInstance);
}

void JumpGuarder::exeHopStart() {
	if (MR::isFirstStep(this)) {
		MR::startAction(mHead, "HopStart");
		MR::startBrk(mHead, "OnAndOff");
		MR::startAction(this, "Damage");
		MR::startLevelSound(this, "EmJguarderHit", -1, -1, -1);
		getSensor("Body")->mRadius = 120.0f;
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
		MR::startAction(mHead, "HopWait");
		MR::validateHitSensor(this, "Jump");
	}

	MR::startLevelSound(this, "EmLvJguarderShake", -1, -1, -1);

	if (MR::isStep(this, 300))
		setNerve(&NrvJumpGuarder::NrvHopEnd::sInstance);
}

void JumpGuarder::exeHopJump() {
	if (MR::isFirstStep(this)) {
		MR::startAction(mHead, "HopJump");
		MR::startLevelSound(this, "EmJguarderTrample", -1, -1, -1);
	}

	if (MR::isActionEnd(mHead))
		setNerve(&NrvJumpGuarder::NrvHopWait::sInstance);
}

void JumpGuarder::exeHopEnd() {
	if (MR::isFirstStep(this)) {
		MR::startAction(this, "HopEnd");
		MR::startAction(mHead, "HopEnd");
		MR::startBrk(mHead, "Green");
		MR::startLevelSound(this, "EmJguarderCloseSpring", -1, -1, -1);
	}

	if (MR::isActionEnd(this) && MR::isActionEnd(mHead)) {
		getSensor("Body")->mRadius = 145.0f;
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
		MR::startAction(this, "Open");
		MR::startLevelSound(this, "EmJguarderShutterOpen", -1, -1, -1);

		// Collect dead BegomanBabys to "revive" them
		mNumPendingBabys = 0;
		for (s32 i = 0; i < mNumBabys; i++) {
			if (MR::isDead(mBabys[i]))
				mPendingBabys[mNumPendingBabys++] = mBabys[i];
		}

		Mtx* headBaseMtx = mHead->getBaseMtx();
		TVec3f ydir, zdir;
		MR::extractMtxYDir(*headBaseMtx, &ydir);
		MR::extractMtxZDir(*headBaseMtx, &zdir);

		for (s32 i = 0; i < mNumPendingBabys; i++) {
			BegomanBaby* baby = mPendingBabys[i];

			MR::rotateVecDegree(&zdir, ydir, 360.0f / mNumPendingBabys);
			baby->mTranslation.set<f32>(mTranslation + zdir * 64.0f);
			baby->mVelocity.setAll<f32>(0.0f);

			appearBabyFromGuarder(baby);
		}
	}
	// Keep BegomanBabys inside until launch
	else if (MR::isLessStep(this, 70)) {
		Mtx* headBaseMtx = mHead->getBaseMtx();
		TVec3f ydir, zdir;
		MR::extractMtxYDir(*headBaseMtx, &ydir);
		MR::extractMtxZDir(*headBaseMtx, &zdir);

		for (s32 i = 0; i < mNumPendingBabys; i++) {
			BegomanBaby* baby = mPendingBabys[i];

			MR::rotateVecDegree(&zdir, ydir, 360.0f / mNumPendingBabys);
			baby->mTranslation.set<f32>(mTranslation + zdir * 64.0f);
			baby->mVelocity.setAll<f32>(0.0f);
		}
	}
	// Finally, launch BegomanBabys
	else if (MR::isStep(this, 70)) {
		Mtx* headBaseMtx = mHead->getBaseMtx();
		TVec3f ydir, zdir;
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
		MR::startAction(this, "Close");

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
bool JumpGuarder::isHit(const LiveActor *pActor) const {
	for (u32 i = 0; i < mNumPendingBabys; i++) {
		if (mPendingBabys[i] == pActor)
			return false;
	}
	return true;
}

void JumpGuarder::appearBabyFromGuarder(BegomanBaby *pBaby) {
	pBaby->BegomanBase::appear();
	pBaby->setNerve(&NrvBegomanBaby::HostTypeNrvLaunchFromGuarder::sInstance);
}

namespace NrvJumpGuarder {
	void NrvHide::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeHide();
	}

	void NrvUp::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeUp();
	}

	void NrvWait::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeWait();
	}

	void NrvDown::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeDown();
	}

	void NrvHopStart::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeHopStart();
	}

	void NrvHopWait::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeHopWait();
	}

	void NrvHopJump::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeHopJump();
	}

	void NrvHopEnd::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeHopEnd();
	}

	void NrvPreOpen::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exePreOpen();
	}

	void NrvOpen::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeOpen();
	}

	void NrvClose::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
		pActor->exeClose();
	}

	void NrvInter::execute(Spine *pSpine) const {
		JumpGuarder* pActor = (JumpGuarder*)pSpine->mExecutor;
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
