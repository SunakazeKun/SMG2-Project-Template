#pragma once

#include "Actor/AreaObj/AreaObj.h"

class WaterArea : public AreaObj {
public:
	WaterArea(const char*);

	virtual void init(const JMapInfoIter&);
};
