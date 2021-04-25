#pragma once

#include "Actor/NameObj/NameObj.h"
#include "Actor/Switch/StageSwitch.h"
#include "JGeometry/TVec3.h"
#include "MapObj/ChipBase.h"

struct ChipStruct {
	ChipBase* mChip;
	u8 _4;
};

class ChipGroup : public NameObj {
public:
	ChipGroup(const char*, s32);

	virtual void init(const JMapInfoIter&);
	virtual void movement();

	void updateUIRange();
	void registerChip(ChipBase*);
	void noticeGet(ChipBase*);
	void noticeEndCompleteDemo();
	void receiveAppearRequest();
	s32 getGotCount();
	bool isComplete();

	ChipStruct mChips[5];
	StageSwitchCtrl* mStageSwitchCtrl;
	JGeometry::TVec3<f32> mTranslate;
	s32 _4C;
	s32 _50;
	s32 mArg0;
	s32 mArg1;
	s32 _5C;
	s32 _60;
	f32 _64;
	f32 mArg2;
	u8 _6C;
	s32 _70;
	u8 _74;
	u8 _75;
};