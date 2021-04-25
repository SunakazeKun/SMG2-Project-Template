#include "spack/Actor/LavaBallRisingPlanetLava.h"
#include "Util/ActorSwitchUtil.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/SoundUtil.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

LavaBallRisingPlanetLava::LavaBallRisingPlanetLava(const char* pName) : MapObjActor(pName) {
	// Instantiate nerves
	for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
		(*f)();

	mMinScale = 1000.0f;
	mMaxScale = 1100.0f;
	mWaitTime = 120;
	mScaleUpTime = 240;
	mScaleDownTime = 240;
	mInitNerve = NERVE_WAIT_SMALL;
	mScaleOnce = false;
	mDoScale = true;
}

void LavaBallRisingPlanetLava::init(const JMapInfoIter& rIter) {
	// Validate MapActor name
	MapObjActor::init(rIter);

	// Load Obj_arg parameters
	MR::getJMapInfoArg0NoInit(rIter, &mMinScale);
	MR::getJMapInfoArg1NoInit(rIter, &mMaxScale);
	MR::getJMapInfoArg2NoInit(rIter, &mWaitTime);
	MR::getJMapInfoArg3NoInit(rIter, &mScaleUpTime);
	MR::getJMapInfoArg4NoInit(rIter, &mScaleDownTime);
	MR::getJMapInfoArg5NoInit(rIter, &mInitNerve);
	MR::getJMapInfoArg6NoInit(rIter, &mScaleOnce);

	// To guarantee precision, the actual scale is multiplied with 1000 when setting the Arg
	mMinScale /= 1000.0f;
	mMaxScale /= 1000.0f;

	// Depending on the initial nerve type, we have to set the proper nerve and scale
	Nerve* nerve;
	f32 scale;

	switch (mInitNerve) {
	default: // NERVE_WAIT_SMALL
		nerve = &NrvLavaBallRisingPlanetLava::NrvWaitSmall::sInstance;
		scale = mMinScale;
		break;
	case NERVE_SCALE_UP:
		nerve = &NrvLavaBallRisingPlanetLava::NrvScaleUp::sInstance;
		scale = mMinScale;
		break;
	case NERVE_WAIT_BIG:
		nerve = &NrvLavaBallRisingPlanetLava::NrvWaitBig::sInstance;
		scale = mMaxScale;
		break;
	case NERVE_SCALE_DOWN:
		nerve = &NrvLavaBallRisingPlanetLava::NrvScaleDown::sInstance;
		scale = mMaxScale;
		break;
	}

	// Initialize MapActor
	MapObjActorInitInfo* initInfo = new MapObjActorInitInfo();
	initInfo->setupConnectToScene();
	initInfo->setupNerve(nerve);
	initialize(rIter, *initInfo);

	// Apply initial scale
	mScale.set<f32>(scale, scale, scale);
}

void LavaBallRisingPlanetLava::exeWait() {
	MR::startLevelSound(this, "Loop", -1, -1, -1);

	if (mDoScale && MR::isStep(this, mWaitTime)) {
		if (isNerve(&NrvLavaBallRisingPlanetLava::NrvWaitSmall::sInstance))
			setNerve(&NrvLavaBallRisingPlanetLava::NrvScaleUp::sInstance);
		else
			setNerve(&NrvLavaBallRisingPlanetLava::NrvScaleDown::sInstance);
	}
}

void LavaBallRisingPlanetLava::exeScaleUp() {
	MR::startLevelSound(this, "Loop", -1, -1, -1);
	MR::startLevelSound(this, "Scale", -1, -1, -1);

	if (mDoScale) {
		f32 scale = MR::calcNerveValue(this, mScaleUpTime, mMinScale, mMaxScale);
		mScale.set(scale, scale, scale);

		if (MR::isStep(this, mScaleUpTime)) {
			setNerve(&NrvLavaBallRisingPlanetLava::NrvWaitBig::sInstance);
			mDoScale = !mScaleOnce;
		}
	}
}

void LavaBallRisingPlanetLava::exeScaleDown() {
	MR::startLevelSound(this, "Loop", -1, -1, -1);
	MR::startLevelSound(this, "Scale", -1, -1, -1);

	if (mDoScale) {
		f32 scale = MR::calcNerveValue(this, mScaleDownTime, mMaxScale, mMinScale);
		mScale.set(scale, scale, scale);

		if (MR::isStep(this, mScaleDownTime)) {
			setNerve(&NrvLavaBallRisingPlanetLava::NrvWaitSmall::sInstance);
			mDoScale = !mScaleOnce;
		}
	}
}

namespace NrvLavaBallRisingPlanetLava {
	void NrvWaitSmall::execute(Spine* spine) const {
		LavaBallRisingPlanetLava* pActor = (LavaBallRisingPlanetLava*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvWaitBig::execute(Spine* spine) const {
		LavaBallRisingPlanetLava* pActor = (LavaBallRisingPlanetLava*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvScaleUp::execute(Spine* spine) const {
		LavaBallRisingPlanetLava* pActor = (LavaBallRisingPlanetLava*)spine->mExecutor;
		pActor->exeScaleUp();
	}

	void NrvScaleDown::execute(Spine* spine) const {
		LavaBallRisingPlanetLava* pActor = (LavaBallRisingPlanetLava*)spine->mExecutor;
		pActor->exeScaleDown();
	}

	NrvWaitSmall(NrvWaitSmall::sInstance);
	NrvWaitBig(NrvWaitBig::sInstance);
	NrvScaleUp(NrvScaleUp::sInstance);
	NrvScaleDown(NrvScaleDown::sInstance);
}
