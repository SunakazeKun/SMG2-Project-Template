#pragma once

#include "MapObj/JetTurtle.h"

class GoldenTurtle : public JetTurtle {
public:
	GoldenTurtle(const char * pName) : JetTurtle(pName) {}

	virtual void exeThrowing();
	virtual void resetPosition();
	virtual void reset(u32);
};
