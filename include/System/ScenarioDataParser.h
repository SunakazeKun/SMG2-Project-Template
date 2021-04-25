#pragma once

class GalaxyStatusAccessor {
public:
	void getScenarioString(const char*, s32, const char**);
};

namespace MR {
	GalaxyStatusAccessor* makeGalaxyStatusAccessor(const char*);
};
