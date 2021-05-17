#include "spack/SPackUtil.h"
#include "JGeometry/TPosition3.h"
#include "JGeometry/TRotation3.h"
#include "Util/LiveActorUtil.h"
#include "Util/MtxUtil.h"
#include "Util/StarPointerUtil.h"
#include "mtx.h"

namespace SPack {
	void clamp(s32 min, s32 max, s32* val) {
		if (max < min) {
			s32 tmp = max;
			max = min;
			min = max;
		}

		if (*val < min)
			*val = min;
		else if (*val > max)
			*val = max;
	}

	void clamp(f32 min, f32 max, f32* val) {
		if (max < min) {
			f32 tmp = max;
			max = min;
			min = max;
		}

		if (*val < min)
			*val = min;
		else if (*val > max)
			*val = max;
	}

	void initPosRotFromRailRider(LiveActor* pActor) {
		f32 coord = pActor->mRailRider->calcNearestPos(pActor->mTranslation);
		pActor->mRailRider->setCoord(coord);
		updatePosRotFromRailRider(pActor);
	}

	void updatePosRotFromRailRider(LiveActor* pActor) {
		// Does actor have RailRider?
		if (pActor->mRailRider == NULL)
			return;

		// Retrieve gravity vector
		TVec3f gravity(0.0f, 1.0f, 0.0f);

		if (MR::isCalcGravity(pActor))
			gravity.set<f32>(-pActor->mGravity);

		// Calculate rotation/position matrix
		TRotationMtx mtx;
		MR::makeMtxUpFront((TPositionMtx*)&mtx, gravity, pActor->mRailRider->mCurrentDirection);

		// Extract euler direction and convert angles to degrees
		TVec3f euler;
		mtx.getEulerXYZ(euler);
		euler.x *= RAD2DEG;
		euler.y *= RAD2DEG;
		euler.z *= RAD2DEG;

		// Update position and rotation
		pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
		pActor->mRotation.set<f32>(euler);
	}
}
