#pragma once

#include "MapObj/MapObjActor.h"

class DeadLeaves : public MapObjActor {
public:
	DeadLeaves(const char*);

	virtual void init(const JMapInfoIter&);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

	void exeSpin();

	s32 mItemType;
};

namespace NrvDeadLeaves {
	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvSpin : public Nerve {
	public:
		NrvSpin() {}
		virtual void execute(Spine*) const;

		static NrvSpin sInstance;
	};
};
