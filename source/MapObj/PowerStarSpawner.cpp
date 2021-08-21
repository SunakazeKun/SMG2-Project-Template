#include "spack/MapObj/PowerStarSpawner.h"
#include "Util.h"

/*
* Created by Aurum, Evanbowl & Someone
* 
* Spawns the specified Power Star if its SW_A is activated. The delay until the star spawns as well
* as the position from where the star spawns can be specified as well.
*/
PowerStarSpawner::PowerStarSpawner(const char *pName) : LiveActor(pName) {
	mScenario = -1;
	mDelay = 0;
	mElapsed = 0;
	mFromMario = false;
}

void PowerStarSpawner::init(JMapInfoIter const &rIter) {
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneMapObjMovement(this);
	MR::invalidateClipping(this);

	MR::useStageSwitchReadA(this, rIter);
	MR::getJMapInfoArg0NoInit(rIter, &mScenario);
	MR::getJMapInfoArg1NoInit(rIter, &mDelay);
	MR::getJMapInfoArg2NoInit(rIter, &mFromMario);

	if (mScenario == -1)
		MR::declarePowerStar(this);
	else
		MR::declarePowerStar(this, mScenario);

	makeActorAppeared();
}

void PowerStarSpawner::movement() {
	if (MR::isOnSwitchA(this)) {
		mElapsed++;

		if (mElapsed >= mDelay) {
			TVec3f spawnpos = mFromMario ? *MR::getPlayerPos() : mTranslation;

			if (mScenario == -1)
				MR::requestAppearPowerStar(this, spawnpos);
			else
				MR::requestAppearPowerStar(this, mScenario, spawnpos);

			makeActorDead();
		}
	}
	else
		mElapsed = 0;
}
