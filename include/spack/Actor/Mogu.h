#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Enemy/AnimScaleController.h"
#include "Actor/Enemy/WalkerStateBindStarPointer.h"
#include "Actor/Model/ModelObj.h"
#include "spack/Actor/MoguStone.h"

class Mogu : public LiveActor {
public:
	Mogu(const char*);

	virtual void init(const JMapInfoIter &);
	virtual void kill();
	virtual void endClipped();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor*, HitSensor*);
	virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

	void exeHideWait();
	void exeHide();
	void exeAppear();
	void exeSearch();
	void exeThrow();
	void tearDownThrow();
	void exeSwoonStart();
	void exeSwoon();
	void exeSwoonEnd();
	void exeHipDropReaction();
	void exeStampDeath();
	void exeHitBlow();
	void exeDPDSwoon();
	void endDPDSwoon();

	bool isPlayerExistUp() const;
	bool isNearPlayerHipDrop() const;
	bool isNerveTypeInactive() const;
	bool tryPunchHitted(HitSensor*, HitSensor*);

	AnimScaleController* mAnimScaleCtrl;
	WalkerStateBindStarPointer* mBindStarPointer;
	MoguStone* mStone;
	ModelObj* mHole;
	TVec3f mFrontVec;
	TVec3f mUpVec;
	bool mCalcStoneGravity;
	bool mIsCannonFleetSight;
};

namespace NrvMogu {
	class NrvHideWait : public Nerve {
	public:
		NrvHideWait() {}
		virtual void execute(Spine *) const;

		static NrvHideWait sInstance;
	};
	class NrvHide : public Nerve {
	public:
		NrvHide() {}
		virtual void execute(Spine *) const;

		static NrvHide sInstance;
	};

	class NrvWait : public Nerve {
	public:
		NrvWait() {}
		virtual void execute(Spine *) const;

		static NrvWait sInstance;
	};

	class NrvAppear : public Nerve {
	public:
		NrvAppear() {}
		virtual void execute(Spine *) const;

		static NrvAppear sInstance;
	};

	class NrvSearch : public Nerve {
	public:
		NrvSearch() {}
		virtual void execute(Spine*) const;

		static NrvSearch sInstance;
	};

	class NrvTurn : public Nerve {
	public:
		NrvTurn() {}
		virtual void execute(Spine*) const;

		static NrvTurn sInstance;
	};

	class NrvThrow : public Nerve {
	public:
		NrvThrow() {}
		virtual void execute(Spine*) const;
		virtual void executeOnEnd(Spine*) const;

		static NrvThrow sInstance;
	};

	class NrvSwoonStart : public Nerve {
	public:
		NrvSwoonStart() {}
		virtual void execute(Spine*) const;

		static NrvSwoonStart sInstance;
	};

	class NrvSwoonEnd : public Nerve {
	public:
		NrvSwoonEnd() {}
		virtual void execute(Spine*) const;

		static NrvSwoonEnd sInstance;
	};

	class NrvSwoon : public Nerve {
	public:
		NrvSwoon() {}
		virtual void execute(Spine*) const;

		static NrvSwoon sInstance;
	};

	class NrvHipDropReaction : public Nerve {
	public:
		NrvHipDropReaction() {}
		virtual void execute(Spine*) const;

		static NrvHipDropReaction sInstance;
	};

	class NrvStampDeath : public Nerve {
	public:
		NrvStampDeath() {}
		virtual void execute(Spine*) const;

		static NrvStampDeath sInstance;
	};

	class NrvHitBlow : public Nerve {
	public:
		NrvHitBlow() {}
		virtual void execute(Spine*) const;

		static NrvHitBlow sInstance;
	};

	class NrvDPDSwoon : public Nerve {
	public:
		NrvDPDSwoon() {}
		virtual void execute(Spine*) const;
		virtual void executeOnEnd(Spine*) const;

		static NrvDPDSwoon sInstance;
	};
};
