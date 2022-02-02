#pragma once

#include "syati.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"

class WAST {
public:

	WAST(bool init);
	void readTable(s32 selectedindex, bool useErrors);

	void selectWipeClose(s32 type, s32 fadeTime);
	void selectWipeOpen(s32 type, s32 fadeTime);

	const char* mDestStageName;
	s32 mDestScenarioNo;
	s32 mDestGreenScenarioNo;
	s32 mBCSVWipeType;
	s32 mBCSVWipeTime;
	s32 mIndex;
	bool mCanWarp;
	ErrorLayout* mErrorLayout;
	};
