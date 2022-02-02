#pragma once

#include "syati.h"
#include "LiveActor/LiveActor.h"

namespace SPack {
	s32 getPowerStarColor(const char *pStage, s32 scenarioId);
	s32 getPowerStarColorCurrentStage(s32 scenarioId);

	void TamakoroCustomPowerStarColors(LiveActor* actor, const JMapInfoIter& iter);

	wchar_t* getStarIcon(wchar_t* unk, s32 type);
};
