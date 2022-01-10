#pragma once

#include "syati.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"

class WarpAreaStageTable {
public:

	WarpAreaStageTable(bool init);
	void readTable(s32 selectedindex, bool useErrors);

	void selectWipeClose(s32 type, s32 fadeTime);
	void selectWipeOpen(s32 type, s32 fadeTime);

	const char* destStage;
	s32 destScenario;
	s32 destGreenStarScenario;
	s32 BCSVWipeType;
	s32 BCSVWipeTime;
	s32 bcsvIndex;
	bool canWarp;
	ErrorLayout* errorLayout;
	};
