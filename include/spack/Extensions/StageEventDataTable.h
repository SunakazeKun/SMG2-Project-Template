#pragma once

#include "syati.h"

namespace SEDT {
    bool StageEventDataTable(const char* value, s32 stageCheck);
	
	bool isPauseDisabled();
	bool isChimp();
	bool isDisableStarChanceBGM();
	bool isPurpleCoinCaretaker();
	bool isStoryBook();
	bool TamakoroSliderBGM();
	void DisableFallFailsafe();
};
