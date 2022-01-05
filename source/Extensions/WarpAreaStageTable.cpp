#include "spack/Extensions/WarpAreaStageTable.h"
#include "Util.h"
#include "Util/StageUtil.h"
#include "spack/Util/ActorUtil.h"
/*
* Authors: Evanbowl
* 
* WarpAreaStageTable:
*
* The other half of WarpArea's functionality.
*
* Reads the BCSV at /SystemData/PTSystemData.arc/WarpAreaStageTable.bcsv to determine what galaxy it should take the player to.
* A loading screen is planned but may take a while to get done.
*/

	WarpAreaStageTable::WarpAreaStageTable() {
		destStage;
		destScenario = 0;
		destGreenStarScenario = 0;
		BCSVFadeInType = 0;
		BCSVFadeInTime = 0;
		bcsvIndex = 0;
	}

	bool warpareaused;
	s32 FadeInType = 0;
	s32 FadeInTime = 45;

	void* WarpAreaStageTableBCSV = SPack::loadArcAndFile("/SystemData/PTSystemData.arc", "/System/WarpAreaStageTable.bcsv");

	void WarpAreaStageTable::readTable(s32 selectedindex) {

		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WarpAreaStageTableBCSV);

		for (s32 i = 0; i < MR::getCsvDataElementNum(StageTable); i++) {

			MR::getCsvDataStr(&destStage, StageTable, "StageName", i);
			MR::getCsvDataS32(&destScenario, StageTable, "ScenarioNo", i);
			MR::getCsvDataS32(&destGreenStarScenario, StageTable, "GreenStarScenarioNo", i);
			MR::getCsvDataS32(&BCSVFadeInType, StageTable, "WipeType", i);
			MR::getCsvDataS32(&BCSVFadeInTime, StageTable, "WipeTime", i);
			MR::getCsvDataS32(&bcsvIndex, StageTable, "Index", i);

		if (selectedindex == bcsvIndex) {

			if (destScenario < 1 || destScenario > 8)
				OSReport("(WarpAreaStageTable) %d is not a valid scenario number. Skipping.\n", destScenario);

			if (destGreenStarScenario < -1 || destGreenStarScenario > 3 || destGreenStarScenario == 0)
				OSReport("(WarpAreaStageTable) %d is not a valid green star scenario number. Skipping.\n", destGreenStarScenario);
			
			FadeInType = BCSVFadeInType; //Separate variables are used to prevent the needed values from being overwritten by the next row in the BCSV.
			FadeInTime = BCSVFadeInTime; //Awful and janky, but it works.

			if (destGreenStarScenario > 0) //Green stars in the WarpAreaStageTable work nicely. Just input a 2, for example, and you'll go to Green Star 2!
				destGreenStarScenario += 3;

			MR::goToGalaxy(destStage);
			MR::goToGalaxyWithoutScenarioSelect(destStage, destScenario, destGreenStarScenario, 0);
			OSReport("(WarpAreaStageTable) Going to %s %d, Green Star %d, Wipe Type: %d, Wipe Time: %d, BCSV Index: %d\n", destStage, destScenario, destGreenStarScenario, BCSVFadeInType, BCSVFadeInTime, bcsvIndex);
			warpareaused = true;
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
		WarpAreaStageTable* WAST = new WarpAreaStageTable();

		if (warpareaused == true) {
			WAST->selectWipeOpen(FadeInType, FadeInTime);
			warpareaused = false;
		}
		else
			MR::openSystemWipeWhiteFade(90);
	}

kmCall(0x804B44D0, setWipeOnStageLoad);