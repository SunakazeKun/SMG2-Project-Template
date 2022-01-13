#include "spack/Extensions/WarpAreaStageTable.h"
#include "Util.h"
#include "Util/StageUtil.h"
#include "spack/Util/ActorUtil.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"

/*
* Authors: Evanbowl
* 
* WarpAreaStageTable:
*
* The other half of WarpArea's functionality.
* Reads the BCSV at /SystemData/PTSystemData.arc/WarpAreaStageTable.bcsv to determine what galaxy it should take the player to.
*/

	WarpAreaStageTable::WarpAreaStageTable(bool init) {
		mDestStageName; //Destination Stage Name
		mDestScenarioNo = 0; //Destionation Scenario Number
		mDestGreenScenarioNo = 0; //Green Star Scenario Number
		mBCSVWipeType = 0; //Wipe Fade In Type (to be copied to "WipeType")
		mBCSVWipeTime = 0; //Wipe Fade In Time (to be copied to "WipeTime")
		mIndex = 0; //BCSV Index
		mCanWarp = true; //Boolean that prevents the warparea transition from happening if it was not set up correctly.
		mErrorLayout = new ErrorLayout();

		if (init)
		mErrorLayout->initWithoutIter();
	}

	bool mWarpAreaUsed;
	s32 mWipeType = 0;
	s32 mWipeTime = 45;

	void* WarpAreaStageTableBCSV = SPack::loadArcAndFile("/SystemData/PTSystemData.arc", "/System/WarpAreaStageTable.bcsv");

	void WarpAreaStageTable::readTable(s32 selectedindex, bool useErrors) {

		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WarpAreaStageTableBCSV);

		for (s32 i = 0; i < MR::getCsvDataElementNum(StageTable); i++) {
			MR::getCsvDataStr(&mDestStageName, StageTable, "StageName", i);
			MR::getCsvDataS32(&mDestScenarioNo, StageTable, "ScenarioNo", i);
			MR::getCsvDataS32(&mDestGreenScenarioNo, StageTable, "GreenStarScenarioNo", i);
			MR::getCsvDataS32(&mBCSVWipeType, StageTable, "WipeType", i);
			MR::getCsvDataS32(&mBCSVWipeTime, StageTable, "WipeTime", i);
			MR::getCsvDataS32(&mIndex, StageTable, "Index", i);

		if (selectedindex == mIndex) {

			if (mDestScenarioNo < 1 || mDestScenarioNo > 8) {
				mErrorLayout->printf(useErrors, "%d is not a valid scenario.\n", mDestScenarioNo); //Print a message on screen if an invalid scenario number is input.
				mCanWarp = false; //Disable warping since the selected row was not set up correctly.
			}

			if (mDestGreenScenarioNo < -1 || mDestGreenScenarioNo > 4 || mDestGreenScenarioNo == 0) {
				mErrorLayout->printf(useErrors, "%d is not a valid green star scenario.\n",  mDestGreenScenarioNo); //Print a message if an invalid green star scenario is input
				mCanWarp = false; 
			}       
			
			mWipeType = mBCSVWipeType; //Separate variables are used to prevent the needed values from being overwritten by the next row in the BCSV.
			mWipeTime = mBCSVWipeTime; //Awful and janky, but it works.

			if (mDestGreenScenarioNo > 0) //Green stars in the WarpAreaStageTable work nicely. Just input a 2, for example, and you'll go to Green Star 2!
				mDestGreenScenarioNo += 3;

			if (mCanWarp) { //If the selected BCSV index is set up correctly, go to the galaxy specified by destStage.
				MR::goToGalaxy(mDestStageName);
				MR::goToGalaxyWithoutScenarioSelect(mDestStageName, mDestScenarioNo, mDestGreenScenarioNo, 0);
				mWarpAreaUsed = true;
				mErrorLayout->kill(); //Make the layout dead since it is not needed anymore.
			}
			else { //Open up the wipe and restore player control if the galaxy transition fails
				MR::openSystemWipeCircle(45);
				MR::onPlayerControl(1);
			}
		}
	}
}

	void WarpAreaStageTable::selectWipeClose(s32 type, s32 fadeTime) {
	if (fadeTime == -1)
		fadeTime = 45;

	switch (type) {
		default:
		case 0:
		MR::closeSystemWipeCircle(fadeTime);
		break;
		case 1:
		MR::closeSystemWipeFade(fadeTime);
		break;
		case 2:
		MR::closeSystemWipeWhiteFade(fadeTime);
		break;
		case 3:
		MR::closeSystemWipeCircleWithCaptureScreen(fadeTime);
		break;
		case 4:
		MR::closeSystemWipeFadeWithCaptureScreen(fadeTime);
		break;
		case 5:
		MR::closeSystemWipeMario(fadeTime);
		break;
		}
	}

	void WarpAreaStageTable::selectWipeOpen(s32 type, s32 fadeTime) {
	if (fadeTime == -1)
		fadeTime = 45;

	switch (type) {
		default:
		case 0:
		MR::openSystemWipeCircle(fadeTime);
		break;
		case 1:
		MR::openSystemWipeFade(fadeTime);
		break;
		case 2:
		MR::openSystemWipeWhiteFade(fadeTime);
		break;
		case 3:
		MR::openSystemWipeMario(fadeTime);
		}
	}

	void setWipeOnStageLoad() {
		WarpAreaStageTable* mStageTable = new WarpAreaStageTable(false);

		if (mWarpAreaUsed == true) {//Checks if the WarpArea was used to enter a galaxy.
		mStageTable->selectWipeOpen(mWipeType, mWipeTime); //If yes, change the opening Wipe to what is set in the selected index's BCSV entry.
			mWarpAreaUsed = false;
		}
		else
			MR::openSystemWipeWhiteFade(90); //If no, use the default wipe and wipe time.
	}

kmCall(0x804B44D0, setWipeOnStageLoad); //sub_804B4490 + 0x40