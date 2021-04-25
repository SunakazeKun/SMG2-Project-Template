#pragma once

#include "kamek.h"
#include "JGeometry/TVec3.h"

class AreaFormCube {
public:
	AreaFormCube(s32);

	void updateBoxParam();

	s32 _4;
	s32 mCubeType;
	JGeometry::TVec3<f32> mTranslation;
	JGeometry::TVec3<f32> mRotation;
	JGeometry::TVec3<f32> mScale;
	JGeometry::TVec3<f32> mVelocity;
	JGeometry::TVec3<f32> mGravity;

	f32 _48, _4C, _50, _54;
	f32 _58, _5C, _60, _64;
	f32 _68, _6C, _70, _74;
};
