#pragma once

#include "syati.h"

class AnimScaleController;
struct AnimScaleParam;
class LiveActor;

namespace SPack {
	const f32 RAD2DEG = 57.295776f;

	void initPosRotFromRailRider(LiveActor *pActor);
	void updatePosRotFromRailRider(LiveActor *pActor);
	AnimScaleController* createSamboAnimScaleController(AnimScaleParam *pAnimScaleParam);
};
