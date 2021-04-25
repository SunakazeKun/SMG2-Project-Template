#pragma once

#include "Screen/LayoutActor.h"

class ChipCounter : public LayoutActor {
public:
	ChipCounter(const char*, s32);

	s32 _2C;
	s32 _30;
	s32 mType;
	s32 _38;
	f32 _3C;
};
