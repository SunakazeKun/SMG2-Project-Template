#pragma once

#include "LiveActor.h"

class ActorCameraInfo;

class PowerStarSpawner : public LiveActor {
public:
	PowerStarSpawner(const char *);

	virtual void init(const JMapInfoIter& rIter);
	virtual void movement();
	virtual void spawnAtMario(f32 offset);
	virtual void createDisplayStar(s32 create);

	ActorCameraInfo* mCamInfo;
	
	s32 mScenario;
	s32 mSpawnMode;
	s32 mDelay;
	bool mUseSuccessSE;
	bool mFromMario;
	s32 mUseDisplayModel;
	f32 mYOffset;
	s32 mGroupID;

	s32 mElapsed;
	ModelObj* DisplayStar;
	TVec3f upVec;
	Mtx DisplayStarMtx;
};
