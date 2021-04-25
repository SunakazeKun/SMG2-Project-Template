#pragma once

#include "Actor/LiveActor/LiveActor.h"

class SpinDriverPathDrawer : public LiveActor {
public:
	void setColorNormal();
	void setColorGreen();
	void setColorPink();

	s32 _90;
	s32 _94;
	s32 _98;
	s32 _9C;
	s32 _A0;
	s32 _A4;
	s32 _A8;
	s32 _AC;
	f32 _B0;
	f32 _B4;
	s32 mColor;
	f32 _BC;
	f32 _C0;
	f32 _C4;
};
