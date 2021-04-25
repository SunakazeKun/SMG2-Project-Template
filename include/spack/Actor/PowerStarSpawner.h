#pragma once

#include "Actor/LiveActor/LiveActor.h"

class PowerStarSpawner : public LiveActor {
public:
	PowerStarSpawner(const char *);

	virtual void init(JMapInfoIter const&);
	virtual void movement();

	s32 mScenario;
	s32 mDelay;
	s32 mElapsed;
	bool mFromMario;
};
