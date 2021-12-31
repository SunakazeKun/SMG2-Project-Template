#include "spack/Extensions/StarChanceExceptTable.h"
#include "custom/archive.h"
#include "Util.h"

/*
* Authors: Aurum
* 
* Everytime a Power Star spawns, the game plays the "Star Chance" theme. While there are a some
* stages that prevent this theme from playing, there is no way to configure this without custom
* code. Therefore, I added a file called StarChanceExceptTable inside the SystemDataTable archive.
* Using this file, you can specify galaxies and scenarios for which the music will change after
* the star spawns. Each entry in the table consists of the StageName and the ScenarioNo.
*/
namespace SPack {
	static void* sStarChanceExceptTableArchive = Syati::loadArchive("/SystemData/StarChanceExceptTable.arc");
	static void* sStarChanceExceptTableBcsv = Syati::loadResourceFromArchive("/SystemData/StarChanceExceptTable.arc", "StarChanceExceptTable.bcsv");
	
	bool isStagePlayStarChance(s32 scenarioId) {
		JMapInfo* exceptTable = new JMapInfo();
		exceptTable->attach(sStarChanceExceptTableBcsv);
	
		s32 numEntries = MR::getCsvDataElementNum(exceptTable);
		const char* currentStage = MR::getCurrentStageName();
	
		for (s32 i = 0; i < numEntries; i++) {
			const char* exceptStage;
			s32 exceptScenario = 0;
			MR::getCsvDataStr(&exceptStage, exceptTable, "StageName", i);
			MR::getCsvDataS32(&exceptScenario, exceptTable, "ScenarioNo", i);
	
			if (MR::isEqualStringCase(currentStage, exceptStage)) {
				if (exceptScenario <= 0 || exceptScenario == scenarioId)
					return false;
			}
		}
	
		return true;
	}
	
	kmWrite32(0x80056E48, 0x7FC3F378); // copy r30 (current scenario number) to r3
	kmCall(0x80056E4C, isStagePlayStarChance);
	kmWrite32(0x80056E50, 0x48000044); // skip redundant checks
}
