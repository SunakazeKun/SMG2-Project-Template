#pragma once

#include "MapObj/MorphItemObjNeo.h"

class MorphItemCollection : public LiveActor {
public:
	MorphItemCollection(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void calcAndSetBaseMtx();

	void exeWait();
	void initPowerUpModel(const JMapInfoIter&);

	s32 mPowerUpType;
	f32 mRotation;
	bool mIsDrill;
};
