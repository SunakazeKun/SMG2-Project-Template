#pragma once

#include "LiveActor.h"

class PowerStarSpawner : public LiveActor {
public:
	PowerStarSpawner(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void movement();

	s32 mScenario;
	s32 mDelay;
	s32 mElapsed;
	bool mFromMario;
};
