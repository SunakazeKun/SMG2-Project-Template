#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Nerve/Nerve.h"
#include "Actor/Enemy/Unizo.h"

class UnizoLauncher : public LiveActor {
public:
	UnizoLauncher(const char*);

	virtual void init(JMapInfoIter const&);

	void exeWait();
	void exeLaunch();

	Unizo** mUnizoArray;
	s32 mUnizoType;
	s32 mNumUnizo;
	s32 mLaunchDelay;
};

namespace NrvUnizoLauncher {
	class NrvWait : public Nerve {
	public:
		NrvWait() {};
		virtual void execute(Spine*) const;

		static NrvWait sInstance;
	};

	class NrvLaunch : public Nerve {
	public:
		NrvLaunch() {};
		virtual void execute(Spine*) const;

		static NrvLaunch sInstance;
	};
};
