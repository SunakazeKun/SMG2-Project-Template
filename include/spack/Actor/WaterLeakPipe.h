#pragma once

#include "Actor/Model/ModelObj.h"

class IceStepNoSlip : public ModelObj {
public:
	IceStepNoSlip(Mtx4*);

	virtual void init(const JMapInfoIter&);
	virtual void appear();

	void exeAppear();
	void exeBreak();
};

class WaterLeakPipe : public LiveActor {
public:
	WaterLeakPipe(const char*);

	virtual void init(const JMapInfoIter&);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

	void exeWait();
	void exeFreeze();
	void initPipeHeight();

	IceStepNoSlip* mIceStep;
	f32 mPipeLength;
	Mtx* mIceStepMtx;
	TVec3f mClippingOffset;
};

namespace NrvIceStepNoSlip {
	class NrvAppear : public Nerve {
	public:
		NrvAppear() {}
		virtual void execute(Spine*) const;

		static NrvAppear sInstance;
	};

	class NrvBreak : public Nerve {
	public:
		NrvBreak() {}
		virtual void execute(Spine*) const;

		static NrvBreak sInstance;
	};
};

namespace NrvWaterLeakPipe {
	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvFreeze : public Nerve {
	public:
		NrvFreeze() {}
		virtual void execute(Spine*) const;

		static NrvFreeze sInstance;
	};
};
