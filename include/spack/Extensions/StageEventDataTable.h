#pragma once

#include "syati.h"

class NameObj;

namespace StageEventDataTable {
    bool StageEventDataTable(const char* value, s32 stageCheck);
	
	bool isPauseDisabled();
	bool isChimp();
	bool isDisableStarChanceBGM();
	bool isPurpleCoinCaretaker();
	bool isStoryBook();
	bool isStageUseTamakoroBGM();
	void isStageDisableFallFailsafe();
};
