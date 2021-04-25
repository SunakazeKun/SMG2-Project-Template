#pragma once

#include "Actor/NameObj/NameObj.h"

#define SCENEOBJ_MIRROR_CAMERA 0x17
#define SCENEOBJ_QUAKE_EFFECT_GENERATOR 0x66
#define SCENEOBJ_YELLOW_CHIP_HOLDER 0x6B

class SceneObjHolder : NameObj {
public:
	NameObj* getObj(int) const;
	NameObj* newEachObj(int);
};

namespace MR {
	NameObj* createSceneObj(int);
	SceneObjHolder* getSceneObjHolder();
	bool isExistSceneObj(int);
};
