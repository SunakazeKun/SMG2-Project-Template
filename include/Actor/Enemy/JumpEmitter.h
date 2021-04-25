#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/PartsModel.h"
#include "JGeometry/TMatrix34.h"

class JumpEmitter : public LiveActor {
public:
	JumpEmitter(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void movement();
	virtual void kill();
	virtual void control();

	void startEventCamera();
	void updateEventCamera();
	void endEventCamera();
	void updateRotate();

	Mtx* mJointMtx;
	Mtx _94;
	PartsModel* mHead;
	s32 _C8; // not in ctor
	f32 _CC;
	f32 _D0;
	f32 _D4;
	u8 _D8;
	s32 _DC;
	s32 _E0;
};

namespace MR {
	bool enableGroupAttack(LiveActor*, f32, f32);
};
