#include "spack/LayoutActor/WarpAreaErrorLayout.h"
#include "Util.h"
#include "c_stdlib.h"

ErrorLayout::ErrorLayout() : LayoutActor("WarpAreaErrorLayout", 0) {
}

void ErrorLayout::init(const JMapInfoIter& rIter) {
	OSReport("init\n");
	MR::connectToSceneLayout(this);
	initLayoutManager("WarpAreaErrorLayout", 0);
	MR::setTextBoxFormatRecursive(this, "TxtText", L"");
	MR::setTextBoxFormatRecursive(this, "ShaText", L"");
	appear();
}

void ErrorLayout::movement() {
}

void ErrorLayout::printf(bool canPrint, const char* format, ...) {
	va_list arg;
	char string[128];

	va_start(arg, format);
	vsnprintf(string, sizeof (string), format, arg);
	va_end(arg);

	if (canPrint) {
		MR::setTextBoxFormatRecursive(this, "TxtText", L"%s", string);
		MR::setTextBoxFormatRecursive(this, "ShaText", L"%s", string);
	}
}


void initCustomLayouts(NameObj* pNameObj) {//You can use this to make custom layouts always visible.
	MR::connectToSceneLayout(pNameObj);
	
	//Example:
	//LayoutName* name = new LayoutName();
	//name->initWithoutIter();
}

kmCall(0x804657A0, initCustomLayouts);
