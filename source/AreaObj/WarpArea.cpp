#include "spack/AreaObj/WarpArea.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"
#include "Util.h"
/*
* Created by Evanbowl with help from Aurum, AwesomeTMC, Galaxy Master, and Zyphro.
* 
* An area that takes Mario to a specified General Position with the name "WarpAreaDestPos".
*
* Not my best code ever, but I've made it look as good as I can.
*
*/

s32 mode;

WarpArea::WarpArea(const char* pName) : AreaObj(pName) {
	mElapsed = 0;
	warp = false;
	pos.set(0.0f, 0.0f, 0.0f);
	stageTable = new WarpAreaStageTable(true); //BCSV table for going to galaxies.
	errorLayout = new ErrorLayout(); //Error layout used for displaying messages on screen if a WarpArea function fails.
}

void WarpArea::init(const JMapInfoIter& rIter) {
	AreaObj::init(rIter);
	MR::connectToSceneAreaObj(this);
	
	if (mObjArg2 < 0)
	mObjArg2 = 45;

	errorLayout->initWithoutIter();
}

void WarpArea::movement() {
	if (isInVolume(*MR::getPlayerPos()))
	warp = true; //This variable prevents the WarpArea stopping if the player exits the area during the transition.
	
	if (warp && stageTable->canWarp == true) { //Checks if you have been in the area, and also if warping is enabled.
		mElapsed++;
		exeWarp();
	}
	else {
		mElapsed = 0;
	}
}

void WarpArea::exeWarp() {
	//Phase 1: Start a circle wipe and lock player control
	if (mElapsed == 10) {
		stageTable->selectWipeClose(mObjArg2, mObjArg3);
		MR::offPlayerControl();
	
	if (mObjArg0 < 0) { //If the player is warping to a galaxy, fade out all music.
		MR::stopSubBGM(45);
		MR::stopStageBGM(45);
	}
}

	//Phase 2: Set the player state to Wait to prevent animation issues and then warp Mario to the specified GeneralPos. Or, go to a galaxy.
	if (mElapsed == mObjArg2 + 60) {
		MR::setPlayerStateWait(); //Sets the player anim to Wait to prevent animation glitches, although some may happen.

		if (mObjArg0 >= 0) {
			char WarpAreaDestPos[0xF];
			sprintf(WarpAreaDestPos, "WarpAreaPos%03d", mObjArg0); //Combines WarpAreaDestPos + the value of mObjArg0.
			MR::setPlayerPosAndWait(WarpAreaDestPos); //Teleports the player to WarpAreaDestPos[mObjArg0].

			if (!MR::findNamePos(WarpAreaDestPos, &pos, &pos)) //Check if the specified general position exists
			errorLayout->printf(mObjArg6, "WarpArea position %s isn't exist.", WarpAreaDestPos); //If not, then print a message on screen.
		}
		else {
			warp = false;
			stageTable->readTable(mObjArg1, mObjArg6); //If no general position is specified, set up a galaxy transition by reading the BCSV.
		}
	}

	//Phase 3: Open the wipe and restore player control.
	if (mElapsed == mObjArg2 + 90) {
		if (mObjArg0 >= 0) {
			stageTable->selectWipeOpen(mObjArg4, mObjArg5);
			MR::onPlayerControl(1);
			mElapsed = 0;
			warp = false;
		}
		else { //This will only appear if a galaxy transition fails, specifically if the specified index is not in the bcsv
			MR::openSystemWipeCircle(45);
			errorLayout->printf(mObjArg6, "BCSV Index %d isn't exist.", mObjArg1);
			MR::onPlayerControl(1);
		}
	}
}

const char* WarpArea::getManagerName() const {
	return "SwitchArea";
}