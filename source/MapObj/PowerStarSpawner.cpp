#include "spack/MapObj/PowerStarSpawner.h"
#include "spack/Extensions/PowerStarColors.h"
#include "Util.h"
#include "Player/MarioAccess.h"

/*
* Created by Aurum, Evanbowl & Someone, with Group ID support thanks to Zyphro.
* 
* Spawns the specified Power Star if its SW_A is activated. The delay until the star spawns as well
* as the position from where the star spawns can be specified as well.
*
* This object is a bit overdeveloped but is one of my favorites.
*
* One of my favorite objects!
*
	Obj_args:
	0: Power Star ID, int
	1: Spawn Type, int
	2: Spawn Delay, int
	3: Use Sound Effect, bool
	4: Use Mario's Position, bool
	5: Use Display Model, int
	6: Y Offset for spawning at the player, float, Default is 300.
*/

PowerStarSpawner::PowerStarSpawner(const char* pName) : LiveActor(pName) {
	mScenario = 1;
	mSpawnMode = -1;
	mDelay = 0;
	mUseSuccessSE = 0;
	mFromMario = 0;
	mUseDisplayModel = -1;
	mYOffset = 300.0f;

	mCamInfo = 0;
	mElapsed = 0;
	mGroupID = -1;
}

void PowerStarSpawner::init(JMapInfoIter const& rIter) {
	MR::initDefaultPos(this, rIter);

	MR::processInitFunction(this, "StarPiece", false);
	MR::hideModel(this); //A model is specified then hidden since it is not neccessary, or else the ModelObj will crash the game.

	MR::connectToSceneMapObj(this);
	MR::invalidateClipping(this); //This object will never unload when offscreen.

	MR::useStageSwitchReadA(this, rIter); //Reads SW_A.
	MR::calcGravity(this);

	MR::getJMapInfoArg0NoInit(rIter, &mScenario); //Star ID
	MR::getJMapInfoArg1NoInit(rIter, &mSpawnMode); //Time Stop/Instant Appear/Squizzard Spawn
	MR::getJMapInfoArg2NoInit(rIter, &mDelay); //Delay before spawn.
	MR::getJMapInfoArg3NoInit(rIter, &mUseSuccessSE); //Play a sound when activated?
	MR::getJMapInfoArg4NoInit(rIter, &mFromMario); //Should the Star start it's spawn path at Mario?
	MR::getJMapInfoArg5NoInit(rIter, &mUseDisplayModel); //Show display model?
	MR::getJMapInfoArg6NoInit(rIter, &mYOffset); //Y Offset if "Spawn At Mario" is used.

	MR::getJMapInfoGroupID(rIter, &mGroupID); //This will cause the PowerStarSpawner to start and end the Power Star's spawn path at Mario.

	initSound(1, "PowerStarSpawner", false, mTranslation); //Initializes Sound

	MR::declarePowerStar(this, mScenario); //Declares the star determined by mScenario.
	makeActorAppeared();

	if (mGroupID >= 0)
	MR::joinToGroupArray(this, rIter, "Star", 0x10), MR::initActorCamera(this, rIter, &mCamInfo);
	//Joins the group array to the star. This allows the PowerStarSpawner to behave like a PowerStarAppearPoint.

	createDisplayStar(mUseDisplayModel); //Creates the Power Star Display model
}

void PowerStarSpawner::movement() {
	if (mFromMario == 1 && mGroupID < 0)
	PowerStarSpawner::spawnAtMario(250); //This is used to bypass an issue where if a star starts it's spawn path at the player, it de-rails the player off of launch star rails.
	
	if (mGroupID >= 0)
	PowerStarSpawner::spawnAtMario(mYOffset); //This function moves the Power Star Spawner to Mario and also puts it above him relative to the current gravity, only if a Group ID is set.
	
	if (mUseDisplayModel == 1)
	MR::rotateMtxLocalYDegree((MtxPtr)&DisplayStarMtx, 3),
	MR::setMtxTrans((MtxPtr)&DisplayStarMtx, mTranslation);
	
	if (mUseDisplayModel == 0)
	upVec.set<f32>(-mGravity),
	DisplayStar->mRotation.set(upVec),
	DisplayStar->mTranslation.set(mTranslation);

	if (MR::isOnSwitchA(this)) {
		mElapsed++;

		if (mElapsed == 1 && mUseSuccessSE)
			MR::startLevelSound(this, "OjPowerStarSpawnerSpawn", -1, -1, -1); //Plays sound.

		if (mElapsed >= mDelay) {
			switch (mSpawnMode) {
				case 0: //time continues during demo
					MR::appearEventPowerStar("PowerStarSpawner", mScenario, &mTranslation, 1, 0, 0);
				break;
				case 1: //star appears instantly
					MR::appearEventPowerStar("PowerStarSpawner", mScenario, &mTranslation, 0, 1, 0);
				break;
				case 2: //squizzard spawn
					MR::appearEventPowerStar("PowerStarSpawner", mScenario, &mTranslation, 0, 0, 1);
				break;
				case 3: //time continues during squizzard spawn demo
					MR::appearEventPowerStar("PowerStarSpawner", mScenario, &mTranslation, 1, 0, 1);
				break;
			}

			if (mUseDisplayModel >= 0)
			DisplayStar->kill();
	
			makeActorDead();
		}
	}
}

void PowerStarSpawner::spawnAtMario(f32 offset) {
	MR::setPosition(this, *MR::getPlayerPos()); //Teleports the PowerStarSpawner to Mario
	JMAVECScaleAdd((Vec*)MarioAccess::getPlayerActor()->getGravityVec(), (Vec*)&mTranslation, (Vec*)&mTranslation, offset*-1);
}

//Display Star Creation
//Creates a "fake" Power Star that looks like a real star.
void PowerStarSpawner::createDisplayStar(s32 create) {
	if (create > -1) {
		DisplayStar = new ModelObj("パワースター", "PowerStar", mUseDisplayModel ? (MtxPtr)DisplayStarMtx : NULL, -2, -2, -2, false);

		MR::setMtxTrans((MtxPtr)DisplayStarMtx, mTranslation); //Set the mtx translation to the PowerStarSpawner's mTranslation.

		MR::emitEffect(DisplayStar, "Light"); //Starts the PowerStar effect "Light" on the DisplayStar.
		MR::invalidateShadowAll(DisplayStar); //Shadows are not needed so they are hidden.

		if (create == 1)
		upVec.set<f32>(-mGravity), //Sets the up vector to what the gravity is. This allows the DisplayStar to calculate it's gravity, like the normal PowerStar.
		MR::makeMtxUpFront((TPositionMtx*)&DisplayStarMtx, upVec, mTranslation);

		//Set up the color to match the PowerStarType
			MR::startBva(DisplayStar, "PowerStarColor");

		if (!MR::hasPowerStarInCurrentStage(mScenario)) { //Checks if you have the specified star. If not, set up the color by setting animation frames.
			s32 frame = SPack::getPowerStarColor(MR::getCurrentStageName(), mScenario);

			MR::startBtp(DisplayStar, "PowerStarColor");
			MR::startBrk(DisplayStar, "PowerStarColor");
			MR::startBtk(DisplayStar, "PowerStarColor");

			MR::setBtpFrameAndStop(DisplayStar, frame);
			MR::setBrkFrameAndStop(DisplayStar, frame);
			MR::setBtkFrameAndStop(DisplayStar, frame);
			MR::setBvaFrameAndStop(DisplayStar, 0);
		}

		DisplayStar->appear();
	}
}