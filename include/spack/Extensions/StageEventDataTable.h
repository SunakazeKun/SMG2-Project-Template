#pragma once

#include "syati.h"

class NerveExecutor;

namespace StageEventDataTable {
    bool StageEventDataTable(const char* value, s32 stageCheck);
	
	bool isPauseDisabled();
	bool isChimp();
	bool isDisableStarChanceBGM();
	bool isPurpleCoinCaretaker();
	bool isStoryBook();
	bool isStageUseTamakoroBGM();
	void isStageDisableFallFailsafe();
	void isStageDisableWorldMapEvents(NerveExecutor* nrv);
};
