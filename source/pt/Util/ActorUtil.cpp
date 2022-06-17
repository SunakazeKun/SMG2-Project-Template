#include "pt/Util/ActorUtil.h"

/*
* Authors: Aurum
*/
namespace pt {
	void moveAndTurnToPlayer(LiveActor *pActor, const MR::ActorMoveParam &rParam) {
		MR::moveAndTurnToPlayer(pActor, rParam._0, rParam._4, rParam._8, rParam._C);
	}

	void moveAndTurnToDirection(LiveActor *pActor, const TVec3f &rDir, const MR::ActorMoveParam &rParam) {
		MR::moveAndTurnToDirection(pActor, rDir, rParam._0, rParam._4, rParam._8, rParam._C);
	}

	void turnToDirectionUpFront(LiveActor *pActor, TVec3f rUp, TVec3f rFront) {
		TMtx34f mtx;
		MR::makeMtxUpFront(&mtx, rUp, rFront);
		mtx.getEulerXYZ(pActor->mRotation);
		pActor->mRotation *= 57.295776f;
	}

	void turnToDirectionGravityFront(LiveActor *pActor, TVec3f rFront) {
		turnToDirectionUpFront(pActor, -pActor->mGravity, rFront);
	}

	void initRailToNearestAndRepositionWithGravity(LiveActor *pActor) {
		RailRider *pRider = pActor->mRailRider;
		pRider->setCoord(pRider->calcNearestPos(pActor->mTranslation));
		getTransRotateFromRailWithGravity(pActor);
	}

	void getTransRotateFromRailWithGravity(LiveActor *pActor) {
		RailRider *pRider = pActor->mRailRider;
		pActor->mTranslation.set(pRider->mCurrentPos);
		turnToDirectionUpFront(pActor, -pActor->mGravity, pRider->mCurrentDir);
	}

	AnimScaleController* createSamboAnimScaleController(AnimScaleParam *pAnimScaleParam) {
		pAnimScaleParam->_10 = 4.0f;
		pAnimScaleParam->_14 = 1.0f;
		pAnimScaleParam->_18 = 0.06f;
		pAnimScaleParam->_1C = 2.0f;
		pAnimScaleParam->_20 = 0.06f;
		pAnimScaleParam->_24 = 0.4f;
		pAnimScaleParam->_28 = 0.8f;

		return new AnimScaleController(pAnimScaleParam);
	}
}
