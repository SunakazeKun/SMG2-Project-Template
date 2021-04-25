#pragma once

#include "MapObj/MapObjActor.h"

#define NERVE_WAIT_SMALL 0
#define NERVE_SCALE_UP 1
#define NERVE_WAIT_BIG 2
#define NERVE_SCALE_DOWN 3

class LavaBallRisingPlanetLava : public MapObjActor {
public:
	LavaBallRisingPlanetLava(const char*);

	virtual void init(const JMapInfoIter&);
	
	void exeWait();
	void exeScaleUp();
	void exeScaleDown();

	f32 mMinScale;
	f32 mMaxScale;
	s32 mWaitTime;
	s32 mScaleUpTime;
	s32 mScaleDownTime;
	s32 mInitNerve;
	bool mScaleOnce;
	bool mDoScale;
};

namespace NrvLavaBallRisingPlanetLava {
	class NrvWaitSmall : public Nerve {
	public:
		NrvWaitSmall() {}
		virtual void execute(Spine*) const;

		static NrvWaitSmall sInstance;
	};

	class NrvWaitBig : public Nerve {
	public:
		NrvWaitBig() {}
		virtual void execute(Spine*) const;

		static NrvWaitBig sInstance;
	};

	class NrvScaleUp : public Nerve {
	public:
		NrvScaleUp() {}
		virtual void execute(Spine*) const;

		static NrvScaleUp sInstance;
	};

	class NrvScaleDown : public Nerve {
	public:
		NrvScaleDown() {}
		virtual void execute(Spine*) const;

		static NrvScaleDown sInstance;
	};
};
