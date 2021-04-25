#pragma once

struct MapObjCreatorEntry {
    const char* pActorName;
    NameObj* (*mCreationFunc)(const char*);
};

class MapObjFactory {
public:
	void* getCreator(const char*);
};