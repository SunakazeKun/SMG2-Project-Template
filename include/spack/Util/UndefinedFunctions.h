#pragma once

#include "syati.h"
#include "NameObj/NameObj.h"

/*
* Undefined Functions
* This allows functions with no known symbol to be used without
* having to create a custom symbol in all the symbol maps.
*/

typedef void (*defGoToStarship) (s32, s32);
typedef s32 (*defGetLastWorldNo) ();
typedef void (*defShowWorldMap) (NameObj*);

struct undefinedFuncs {
	defGoToStarship goToStarship;
    defGetLastWorldNo getLastWorldNo;
	defShowWorldMap showWorldMap;
};

const undefinedFuncs unkFuncs = {
	(defGoToStarship) 0x804D63F0,
    (defGetLastWorldNo) 0x804D3F50,
	(defShowWorldMap) 0x804EF3A0,
};