#pragma once

#include "syati.h"

class WarpAreaStageTable {
public:

	WarpAreaStageTable();
	void readTable(s32 selectedindex);

	void selectWipeClose(s32 type, s32 fadeTime);
	void selectWipeOpen(s32 type, s32 fadeTime);

	const char* destStage;
	s32 destScenario;
	s32 destGreenStarScenario;
	s32 BCSVFadeInType;
	s32 BCSVFadeInTime;
	s32 bcsvIndex;

	s32 FadeInType;
	s32 FadeInTime;
	};
