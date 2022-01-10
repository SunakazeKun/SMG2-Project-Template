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
		destStage; //Destination Stage Name
		destScenario = 0; //Destionation Scenario Number
		destGreenStarScenario = 0; //Green Star Scenario Number
		BCSVWipeType = 0; //Wipe Fade In Type (to be copied to "WipeType")
		BCSVWipeTime = 0; //Wipe Fade In Time (to be copied to "WipeTime")
		bcsvIndex = 0; //BCSV Index
		canWarp = true; //Boolean that prevents the warparea transition from happening if it was not set up correctly.
		errorLayout = new ErrorLayout();

		if (init)
		errorLayout->initWithoutIter();
	}

	bool warpareaused;
	s32 WipeType = 0;
	s32 WipeTime = 45;

	void* WarpAreaStageTableBCSV = SPack::loadArcAndFile("/SystemData/PTSystemData.arc", "/System/WarpAreaStageTable.bcsv");

	void WarpAreaStageTable::readTable(s32 selectedindex, bool useErrors) {

		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WarpAreaStageTableBCSV);

		for (s32 i = 0; i < MR::getCsvDataElementNum(StageTable); i++) {
			MR::getCsvDataStr(&destStage, StageTable, "StageName", i);
			MR::getCsvDataS32(&destScenario, StageTable, "ScenarioNo", i);
			MR::getCsvDataS32(&destGreenStarScenario, StageTable, "GreenStarScenarioNo", i);
			MR::getCsvDataS32(&BCSVWipeType, StageTable, "WipeType", i);
			MR::getCsvDataS32(&BCSVWipeTime, StageTable, "WipeTime", i);
			MR::getCsvDataS32(&bcsvIndex, StageTable, "Index", i);

		if (selectedindex == bcsvIndex) {

			if (destScenario < 1 || destScenario > 8) {
				errorLayout->printf(useErrors, "%d is not a valid scenario.\n", destScenario); //Print a message on screen if an invalid scenario number is input.
				canWarp = false; //Disable warping since the selected row was not set up correctly.
			}

			if (destGreenStarScenario < -1 || destGreenStarScenario > 4 || destGreenStarScenario == 0) {
				errorLayout->printf(useErrors, "%d is not a valid green star scenario.\n", destGreenStarScenario); //Print a message if an invalid green star scenario is input
				canWarp = false; 
			}
			
			WipeType = BCSVWipeType; //Separate variables are used to prevent the needed values from being overwritten by the next row in the BCSV.
			WipeTime = BCSVWipeTime; //Awful and janky, but it works.

			if (destGreenStarScenario > 0) //Green stars in the WarpAreaStageTable work nicely. Just input a 2, for example, and you'll go to Green Star 2!
				destGreenStarScenario += 3;

			if (canWarp) { //If the selected BCSV index is set up correctly, go to the galaxy specified by destStage.
				MR::goToGalaxy(destStage);
				MR::goToGalaxyWithoutScenarioSelect(destStage, destScenario, destGreenStarScenario, 0);
				warpareaused = true;
				errorLayout->kill();
			}
			else {
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
		WarpAreaStageTable* wast = new WarpAreaStageTable(false);

		if (warpareaused == true) {//Checks if the WarpArea was used to enter a galaxy.
			wast->selectWipeOpen(WipeType, WipeTime); //If yes, change the opening Wipe to what is set in the selected index's BCSV entry.
			warpareaused = false;
		}
		else
			MR::openSystemWipeWhiteFade(90); //If no, use the default wipe and wipe time.
	}

kmCall(0x804B44D0, setWipeOnStageLoad); //sub_804B4490 + 0x40