#include "spack/Extensions/StageEventDataTable.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "Util.h"
#include "spack/Util/ActorUtil.h"
#include "Screen/LayoutActor.h"
#include "System/GameSequenceFunction.h"
#include "Util/StageUtil.h"
#include "spack/Util/UndefinedFunctions.h"

/*
* Authors: Evanbowl
* 
* StageEventDataTable:
*
* Code optimation help by Galaxy Master
*
* These functions un-hardcode certain options a stage can have. Those being:
* Chimp, Glider, Disable Pause, Play Star Chance, Purple Coin Over 100, and Story Book.
*/

namespace SEDT {

	const char* flags;
    void* StageEventDataTableBCSV = SPack::loadArcAndFile("/SystemData/PTSystemData.arc", "/System/StageEventDataTable.bcsv");
		
	//StageEventDataTable Parser
	bool StageEventDataTable(const char* value, s32 stageCheck) {
		const char* stage;

		if (stageCheck == 1)
			stage = GameSequenceFunction::getClearedStageName();
		else
			stage = MR::getCurrentStageName();

		JMapInfo* StageDataTable = new JMapInfo();
		StageDataTable->attach(StageEventDataTableBCSV);

		for (s32 i = 0; i < MR::getCsvDataElementNum(StageDataTable); i++) {
			const char *exceptStage = 0;
			s32 exceptScenario = 0;

			MR::getCsvDataStr(&exceptStage, StageDataTable, "StageName", i);
			MR::getCsvDataS32(&exceptScenario, StageDataTable, "ScenarioNo", i);
			MR::getCsvDataStr(&flags, StageDataTable, "Flags", i);

			if (!flags)
				OSReport("(StageEventDataTable) \"Flags\" is null.");

			if ((exceptScenario == 0 || exceptScenario == MR::getCurrentScenarioNo()) && MR::isEqualStringCase(stage, exceptStage) && strstr(flags, value))
				return true;
		}
		return false;
	}

    /*
    * Makes the set galaxy a Chimp stage. If you apply this to a non-chimp star, the high scores will not work.
    * If the high scores did work, that would mean that the save file would only work on hacks with PT.
    */

	bool isChimp() {
		return SEDT::StageEventDataTable("Chimp", 0); 
	}

	bool isPauseDisabled() {
		return SEDT::StageEventDataTable("DisablePause", 0); //Completely disables the pause menu in the set galaxy.
	}

	bool isGlider() {
		return SEDT::StageEventDataTable("Glider", 0); //Makes the set galaxy a Glider stage.
	}

	bool isDisableStarChanceBGM() {
		return !SEDT::StageEventDataTable("DisableStarChanceBGM", 0); //Makes the set galaxy not set the BGM to star chance after a star spawn.
	}

	bool isPurpleCoinCaretaker() {
		return SEDT::StageEventDataTable("PurpleCoinsOver100", 0); //Makes purple coins in the set galaxy behave like Tall Trunk/Rolling Coaster.
	}
	
	bool isStoryBook() {
	if (MR::isEqualStageName("FileSelect")) //FileSelect will crash if MR::isStageStoryBook returns true on this stage.
		return false;
	return SEDT::StageEventDataTable("StoryBook", 0); //Makes the set galaxy a Story Book stage, adding a story book border and preventing return to the Starship.
	}
	
	bool TamakoroSliderBGM() { //Makes the set galaxy play the Slider music when a Star Ball is jumped on.
		return SEDT::StageEventDataTable("TamakoroSliderBGM", 0);
	}

	void DisableFallFailsafe() { //Makes the set stage disable the game's failsafe that kills the player if falling for too long.
	if (!SEDT::StageEventDataTable("DisableFallFailsafe", 0))
			MR::forceKillPlayerByAbyss();
	}

	void DisableWorldMapEvents(NameObj* obj) {
		if (!SEDT::StageEventDataTable("DisableWorldMapEvents", 1))
			unkFuncs.showWorldMap(obj);
	}

	kmBranch(0x800568F0, isChimp);

	kmBranch(0x80056B40, isPauseDisabled);

	kmBranch(0x800570C0, isGlider);

	kmBranch(0x80056DF0, isDisableStarChanceBGM);

	kmBranch(0x80056D70, isPurpleCoinCaretaker);

	kmBranch(0x80056BE0, isStoryBook);

	kmCall(0x804477B4, TamakoroSliderBGM);
	kmWrite32(0x800857EC, 0x38600000);	

	kmCall(0x80387F64, DisableFallFailsafe);

	kmCall(0x8036B6BC, DisableWorldMapEvents);
	kmCall(0x80495EB0, DisableWorldMapEvents);
	kmCall(0x80495F10, DisableWorldMapEvents);
	kmCall(0x80496CB0, DisableWorldMapEvents);
	kmCall(0x80496D00, DisableWorldMapEvents);
}