#pragma once

#include "AreaObj.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "spack/LayoutActor/WarpAreaErrorLayout.h"

class WarpArea : public AreaObj {
public:
	WarpArea(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void movement();
	virtual void exeWarp();
	virtual const char* getManagerName() const;

	s32 mElapsed;
	bool mCanWarp;
	TVec3f mPos;
	WarpAreaStageTable* mStageTable;
	ErrorLayout* mErrorLayout;
};
