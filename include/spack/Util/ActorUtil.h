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
	void useTimerSE(LiveActor* pActor, s32 pTime);
	void* loadArcAndFile(const char* pArc, const char* pFile);
};
