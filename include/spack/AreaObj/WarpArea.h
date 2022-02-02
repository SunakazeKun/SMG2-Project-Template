#pragma once

#include "AreaObj.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"

class WarpArea : public AreaObj {
public:
	WarpArea(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void movement();
	virtual const char* getManagerName() const;

	s32 mElapsed;
	bool mCanWarp;
	TVec3f mPos;
	WAST* mStageTable;
	ErrorLayout* mErrorLayout;

    //Obj_args are defined this way to make the code easier to read
	#define mPosID mObjArg0
	#define mIndex mObjArg1
	#define mFadeCloseType mObjArg2
	#define mFadeCloseTime mObjArg3
	#define mFadeOpenType mObjArg4
	#define mFadeOpenTime mObjArg5
	#define mPrintErrors mObjArg6
};
