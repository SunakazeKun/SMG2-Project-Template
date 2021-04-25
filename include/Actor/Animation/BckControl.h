#pragma once

#include "Actor/Animation/BckCtrlData.h"

class ResourceHolder;

class BckControl : public BckCtrlData {
public:
	BckControl(ResourceHolder*, const char*);

	f32 _10;
	s32 _14;
	s32 _18;
	s32 _1C;
};
