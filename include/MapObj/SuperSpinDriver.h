#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "JGeometry/TMatrix34.h"
#include "JGeometry/TVec3.h"
#include "MapObj/SpinDriverPathDrawer.h"

#define SUPER_SPIN_DRIVER_YELLOW 0
#define SUPER_SPIN_DRIVER_GREEN 1
#define SUPER_SPIN_DRIVER_PINK 2

class SuperSpinDriver : public LiveActor {
public:
	SuperSpinDriver(const char*, s32);

	void initColor();

	s32 _90;
	s32 _94;
	s32 _98;
	SpinDriverPathDrawer* mSpinDriverPathDrawer;
	s32 _A0;
	s32 _A4;

	f32 _A8, _AC, _B0, _B4;
	f32 _B8, _BC, _C0, _C4;
	f32 _C8, _CC, _D0, _D4;

	s32 _D8, _DC, _E0, _E4;
	s32 _E8, _EC, _F0, _F4;
	JGeometry::TVec3<f32> _F8;
	JGeometry::TVec3<f32> _104;
	JGeometry::TVec3<f32> _110;
	JGeometry::TVec3<f32> _11C;
	JGeometry::TVec3<f32> _128;
	JGeometry::TVec3<f32> _134;
	JGeometry::TVec3<f32> _140;
	JGeometry::TVec3<f32> _14C;
	JGeometry::TVec3<f32> _158;
	s32 _164;
	f32 _168;
	f32 _16C;
	f32 _170;
	f32 _174;
	f32 _178;
	f32 _17C;
	f32 _180;
	s32 _184;
	s32 _188;
	s32 _18C;
	s32 _190;
	s32 _194;
	f32 _198;
	u8 _19C;
	s32 _1A0;
	s32 mColor;
	s32 _1A8;
	u8 _1AC;
	s32 _1B0;
	u8 _1B4;
	u8 _1B5;
	u8 _1B6;
	u8 _1B7;
};
