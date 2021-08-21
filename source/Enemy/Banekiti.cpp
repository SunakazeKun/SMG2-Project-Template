#include "spack/Enemy/Banekiti.h"
#include "Util.h"

/*
* Authors: Aurum
* Objects: Banekiti
* Parameters:
*  - path
*  - SW_AWAKE
* 
* This is Banekiti, sometimes also referred to as Li'l Springy, a yellow oval-shaped obstacle with
* four springs on its sides. It serves as an obstacle for Boo Mario as it bounces him back when he
* touches any of the springs. Boo Mario is underused in SMG2, so this can be used for some clever
* level setups in the future.
*/

Banekiti::Banekiti(const char *pName) : LiveActor(pName) {
	mAnimScaleCtrl = NULL;
	mBindStarPointer = NULL;
	mRailMover = NULL;
}

void Banekiti::init(const JMapInfoIter &rIter) {
	MR::processInitFunction(this, "Banekiti", false);
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneMapObjStrongLight(this);

	// Initialize sensors
	initHitSensor(3);
	MR::addHitSensorMapObjSimple(this, "Body", 4, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
	MR::addHitSensorMapObjSimple(this, "Right", 4, 80.0f, TVec3f(-100.0f, 0.0f, 0.0f));
	MR::addHitSensorMapObjSimple(this, "Left", 4, 80.0f, TVec3f(100.0f, 0.0f, 0.0f));

	// Initialize effects, sounds and light
	initEffectKeeper(1, NULL, false);
	initSound(4, "Banekiti", false, TVec3f(0.0f, 0.0f, 0.0f));
	MR::initLightCtrl(this);

	// Initialize path movement
	initRailRider(rIter);
	mRailMover = new MapPartsRailMover(this);
	mRailMover->init(rIter);
	mRailMover->start();

	// Initialize 2P behavior
	MR::initStarPointerTarget(this, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
	mAnimScaleCtrl = new AnimScaleController(NULL);
	mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);

	initNerve(&NrvBanekiti::NrvWait::sInstance, 0);

	MR::useStageSwitchAwake(this, rIter);
	makeActorAppeared();
}

void Banekiti::control() {
	mAnimScaleCtrl->update();

	// Try to move the rail mover and copy its position
	mRailMover->movement();

	if (mRailMover->isWorking())
		MR::setPosition(this, mRailMover->mTranslation);

	// Try to update P2 behavior
	if (!isNerve(&NrvBanekiti::NrvDPDSwoon::sInstance)) {
		if (mBindStarPointer->tryStartPointBind())
			setNerve(&NrvBanekiti::NrvDPDSwoon::sInstance);
	}
}

void Banekiti::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	MR::updateBaseScale(this, mAnimScaleCtrl);
}

bool Banekiti::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
	if (MR::isMsgStarPieceAttack(msg)) {
		mAnimScaleCtrl->startHitReaction();
		return true;
	}

	return false;
}

bool Banekiti::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *) {
	if (MR::isSensorPlayer(pSender) && !isNerve(&NrvBanekiti::NrvRepel::sInstance)) {
		if (msg == ACTMES_TERESA_PLAYER_TOUCH) {
			setNerve(&NrvBanekiti::NrvRepel::sInstance);
			return true;
		}
	}

	return false;
}

void Banekiti::exeWait() {
	if (MR::isFirstStep(this))
		MR::startAction(this, "Wait");

	MR::startLevelSound(this, "OjLvBanekitiMove", -1, -1, -1);
}

void Banekiti::exeRepel() {
	if (MR::isFirstStep(this)) {
		MR::tryRumblePadWeak(this, 0);
		MR::shakeCameraWeak();

		MR::startAction(this, "Repel");
		MR::startLevelSound(this, "OjBanekitiRepel", -1, -1, -1);
	}

	if (MR::isActionEnd(this))
		setNerve(&NrvBanekiti::NrvWait::sInstance);
}

void Banekiti::exeDPDSwoon() {
	if (MR::isFirstStep(this))
		mRailMover->mEnabled = false;

	MR::updateActorStateAndNextNerve((LiveActor*)this, mBindStarPointer, &NrvBanekiti::NrvWait::sInstance);
}

void Banekiti::endDPDSwoon() {
	mRailMover->mEnabled = true;
	mBindStarPointer->kill();
}

namespace NrvBanekiti {
	void NrvWait::execute(Spine *pSpine) const {
		Banekiti* pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeWait();
	}

	void NrvRepel::execute(Spine *pSpine) const {
		Banekiti* pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeRepel();
	}

	void NrvDPDSwoon::execute(Spine *pSpine) const {
		Banekiti* pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeDPDSwoon();
	}

	void NrvDPDSwoon::executeOnEnd(Spine *pSpine) const {
		Banekiti* pActor = (Banekiti*)pSpine->mExecutor;
		pActor->endDPDSwoon();
	}

	NrvWait(NrvWait::sInstance);
	NrvRepel(NrvRepel::sInstance);
	NrvDPDSwoon(NrvDPDSwoon::sInstance);
}
