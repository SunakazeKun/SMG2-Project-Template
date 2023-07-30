#include "pt/MapObj/PowerStarSpawner.h"

/*
* Authors: Aurum, Evanbowl & Someone
* Objects: PowerStarSpawner
* Parameters:
*  - Obj_arg0, int, -1: Power Star ID
*  - Obj_arg1, int, 0: Spawn delay
*  - Obj_arg2, bool: Appear from player?
*  - Obj_arg3, bool: Play fanfare?
*  - SW_A, use, read: Spawns Power Star when activated
* 
* Spawns the specified Power Star if its SW_A is activated. The delay until the star spawns as well as the position from
* where the star spawns can be specified as well.
*/

namespace pt {
	PowerStarSpawner::PowerStarSpawner(const char *pName) : LiveActor(pName) {
		mScenario = -1;
		mDelay = 0;
		mFromMario = false;
		mPlayFanfare = false;
	}

	void PowerStarSpawner::init(JMapInfoIter const &rIter) {
		MR::initDefaultPos(this, rIter);
		MR::connectToSceneMapObjMovement(this);

		MR::getJMapInfoArg0NoInit(rIter, &mScenario);
		MR::getJMapInfoArg1NoInit(rIter, &mDelay);
		MR::getJMapInfoArg2NoInit(rIter, &mFromMario);
		MR::getJMapInfoArg3NoInit(rIter, &mPlayFanfare);

		MR::needStageSwitchReadA(this, rIter);
		MR::listenStageSwitchOnA(this, MR::Functor(this, PowerStarSpawner::startSpawn));

		initNerve(NrvPowerStarSpawnerWait, 0);

		if (mDelay < 0) {
			mDelay = 0;
		}

		if (mScenario == -1) {
			MR::declarePowerStar(this);
		}
		else {
			MR::declarePowerStar(this, mScenario);
		}

		if (mPlayFanfare) {
			initSound(2, "ExterminationGeneralChecker", NULL, TVec3f(0.0f, 0.0f, 0.0f));
		}

		makeActorAppeared();
	}

	void PowerStarSpawner::startSpawn() {
		if (isNerve(NrvPowerStarSpawnerWait)) {
			MR::invalidateClipping(this);

			if (mPlayFanfare) {
				setNerve(NrvPowerStarSpawnerPrepare);
			}
			else {
				setNerve(NrvPowerStarSpawnerSpawn);
			}
		}
	}

	void PowerStarSpawner::exePrepare() {
		if (MR::isFirstStep(this)) {
			MR::startActionSound(this, "CompleteInstant", -1, -1, -1);
		}

		if (MR::isStep(this, 105)) {
			setNerve(NrvPowerStarSpawnerSpawn);
		}
	}

	void PowerStarSpawner::exeSpawn() {
		if (MR::isStep(this, mDelay)) {
			TVec3f spawnpos = mFromMario ? *MR::getPlayerPos() : mTranslation;

			if (mScenario == -1) {
				MR::requestAppearPowerStar(this, spawnpos);
			}
			else {
				MR::requestAppearPowerStar(this, mScenario, spawnpos);
			}

			kill();
		}
	}

	namespace NrvPowerStarSpawner {
		void NrvWait::execute(Spine *pSpine) const {}

		void NrvPrepare::execute(Spine *pSpine) const {
			PowerStarSpawner *pActor = (PowerStarSpawner*)pSpine->mExecutor;
			pActor->exePrepare();
		}

		void NrvSpawn::execute(Spine *pSpine) const {
			PowerStarSpawner *pActor = (PowerStarSpawner*)pSpine->mExecutor;
			pActor->exeSpawn();
		}

		NrvWait(NrvWait::sInstance);
		NrvPrepare(NrvPrepare::sInstance);
		NrvSpawn(NrvSpawn::sInstance);
	}
}
