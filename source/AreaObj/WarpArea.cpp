#include "spack/AreaObj/WarpArea.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "Util.h"
/*
* Created by Evanbowl with help from Aurum, AwesomeTMC, Galaxy Master, and Zyphro.
* 
* An area that takes Mario to a specified General Position with the name "WarpAreaDestPos".
* It can also take you to galaxies using the WarpAreaStageTable BCSV file.
*/

WarpArea::WarpArea(const char* pName) : AreaObj(pName) {
	mElapsed = 0;
	warp = false;
	pos.set(0.0f, 0.0f, 0.0f);
	StageTable = new WarpAreaStageTable();
}

void WarpArea::init(const JMapInfoIter& rIter) {
	AreaObj::init(rIter);
	MR::connectToSceneAreaObj(this);
	if (mObjArg2 < 0)
	mObjArg2 = 45;
}

void WarpArea::movement() {
    if (this->isInVolume(*MR::getPlayerPos()))
    warp = true;
    
	if (warp) {
		mElapsed++;
		this->exeWarp();
	}
	else
		mElapsed = 0;
}

void WarpArea::exeWarp() {

	if (mElapsed == 10) { //Phase 1: Start a circle wipe and lock player control
	    StageTable->selectWipeClose(mObjArg2, mObjArg3);
		MR::offPlayerControl();
	
	if (mObjArg0 < 0) {
		MR::stopSubBGM(45);
		MR::stopStageBGM(45);
	}
}

if (mElapsed == mObjArg2 + 60) { //Phase 2: Set the player state to Wait to prevent animation issues and then warp Mario to the specified GeneralPos. Or, go to a galaxy.
	MR::setPlayerStateWait();

	if (mObjArg0 >= 0) {
		char WarpAreaDestPos[0xF];
		sprintf(WarpAreaDestPos, "WarpAreaPos%03d", mObjArg0);

		if (MR::findNamePos(WarpAreaDestPos, &pos, &pos))
		MR::setPlayerPos(WarpAreaDestPos);
		else
		MR::setPlayerPos(TVec3f(0.0f, 0.0f, 0.0f));
		}
	else 
	    warp = false,
		StageTable->readTable(mObjArg1); //If no general position is specified, instead set up a galaxy transition by reading the BCSV.
	}

	if (mElapsed == mObjArg2 + 90) {
		StageTable->selectWipeOpen(mObjArg4, mObjArg5);
		MR::onPlayerControl(1);
		mElapsed = 0;
		warp = false;
		}
	}

const char* WarpArea::getManagerName() const {
	return "SwitchArea";
}