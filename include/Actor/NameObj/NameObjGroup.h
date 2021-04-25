#pragma once

#include "Actor/NameObj/NameObj.h"

class NameObjGroup : public NameObj {
public:
	NameObjGroup(const char*, int);

	void registerObj(NameObj*);
	void pauseOffAll() const;
	void initObjArray(int);

	s32 mSize;
	s32 _10;
	NameObj** mArray;
};
