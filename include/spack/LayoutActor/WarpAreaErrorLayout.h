#pragma once

#include "syati.h"
#include "Util.h"
#include "Screen/LayoutActor.h"

class ErrorLayout : public LayoutActor {
public:
	ErrorLayout();
	
	virtual void init (const JMapInfoIter& rIter);
	virtual void movement();

	void printf(bool canPrint, const char* format, ...);

};