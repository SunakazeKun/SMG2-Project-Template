#include "spack/Actor/UnizoLauncher.h"
#include "spack/SPackUtil.h"
#include "Util/ActorSensorUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/JMapUtil.h"
#include "Util/ObjUtil.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

UnizoLauncher::UnizoLauncher(const char* pName) : LiveActor(pName) {
	// Instantiate nerves
	for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
		(*f)();

	mUnizoArray = NULL;
	mUnizoType = UNIZO;
	mNumUnizo = 4;
	mLaunchDelay = 300;
}

void UnizoLauncher::init(JMapInfoIter const& rIter) {
	MR::initDefaultPos(this, rIter);
	MR::connectToSceneEnemyMovement(this);
	initNerve(&NrvUnizoLauncher::NrvWait::sInstance, 0);

	MR::getJMapInfoArg0NoInit(rIter, &mUnizoType);
	MR::getJMapInfoArg1NoInit(rIter, &mNumUnizo);
	MR::getJMapInfoArg2NoInit(rIter, &mLaunchDelay);
	SPack::clamp(UNIZO, UNIZO_SHOAL, &mUnizoType);
	SPack::clamp(1, 8, &mNumUnizo);

	mUnizoArray = new Unizo*[mNumUnizo];
	MR::setGroupClipping(this, rIter, 16);

	for (int i = 0; i < mNumUnizo; i++) {
		Unizo* unizo = new Unizo("ウニゾー");
		unizo->mType = mUnizoType;
		unizo->init(rIter);
		unizo->makeActorDead();
		mUnizoArray[i] = unizo;
		MR::setGroupClipping(unizo, rIter, 16);
	}

	makeActorAppeared();
}

void UnizoLauncher::exeWait() {
	if (MR::isGreaterStep(this, mLaunchDelay))
		setNerve(&NrvUnizoLauncher::NrvLaunch::sInstance);
}

void UnizoLauncher::exeLaunch() {
	if (MR::isFirstStep(this)) {
		for (int i = 0; i < mNumUnizo; i++) {
			Unizo* unizo = mUnizoArray[i];

			if (MR::isDead(unizo)) {
				MR::copyTransRotateScale(this, unizo);
				unizo->mTranslation.z -= 1.0f * i;
				unizo->appear();
				break;
			}
		}
	}

	setNerve(&NrvUnizoLauncher::NrvWait::sInstance);
}

namespace NrvUnizoLauncher {
	void NrvWait::execute(Spine* spine) const {
		UnizoLauncher* pActor = (UnizoLauncher*)spine->mExecutor;
		pActor->exeWait();
	}

	void NrvLaunch::execute(Spine* spine) const {
		UnizoLauncher* pActor = (UnizoLauncher*)spine->mExecutor;
		pActor->exeLaunch();
	}

	NrvWait(NrvWait::sInstance);
	NrvLaunch(NrvLaunch::sInstance);
}
