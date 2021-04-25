#pragma once

#include "Actor/NameObj/NameObj.h"

class LayoutActor : public NameObj {
public:
	LayoutActor(const char*, bool);

	void initLayoutManager(const char*, u32);

	s32 _14;
	s32 _18;
	s32 _1C;
	s32 _20;
	s32 _24; // LayoutActorFlag
	s32 _28;
};
