#pragma once

#include "Actor/NameObj/NameObjGroup.h"

class LiveActorGroup : public NameObjGroup {
public:
	LiveActorGroup(const char*, int);

	void registerActor(LiveActor*);
	LiveActor* getActor(int) const;
	LiveActor* getDeadActor() const;
	s32 getLivingActorNum() const;
	void appearAll();
	void killAll();
};
