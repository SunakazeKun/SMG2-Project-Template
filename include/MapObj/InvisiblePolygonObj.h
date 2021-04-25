#pragma once

#include "Actor/LiveActor/LiveActor.h"

class InvisiblePolygonObj : public LiveActor {
public:
	InvisiblePolygonObj(const char*);

	virtual void init(const JMapInfoIter&);
	virtual Mtx* getBaseMtx() const;

	void initBaseMtx();
	void initCollision(const JMapInfoIter&);

	f32 _8C;
	f32 _90;
	f32 _94;
	f32 _98;
	f32 _9C;
	f32 _A0;
	f32 _A4;
	f32 _A8;
	f32 _AC;
	f32 _B0;
	f32 _B4;
	f32 _B8;
};