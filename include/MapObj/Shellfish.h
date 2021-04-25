#pragma once

#include "Actor/LiveActor/LiveActor.h"

class Shellfish : public LiveActor {
public:
	Shellfish(const char*);

	void initItem(const JMapInfoIter&);
	void putItem();

	s32 _90;
	s32 _94;
	LiveActor* mItem;
	s32 mItemType;
	u8 _A0;
	s32 _A4;
};
