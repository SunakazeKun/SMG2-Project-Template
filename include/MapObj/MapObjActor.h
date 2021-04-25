#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "MapObj/MapObjActorInitInfo.h"

class MapObjActor : public LiveActor {
public:
	MapObjActor(const char*);

	virtual void init(const JMapInfoIter&);
	virtual void initAfterPlacement();
	virtual void appear();
	virtual void kill();
	virtual void startClipped();
	virtual void endClipped();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void func_802C9C60();
	virtual void connectToScene(const MapObjActorInitInfo&);
	virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
	virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
	virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
	virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
	virtual void func_802565B0();
	virtual void func_802CB460();
	virtual void func_802CB0D0();
	virtual void func_802CB170();

	void initialize(const JMapInfoIter&, const MapObjActorInitInfo&);
	void exeWait();

	s32 _90;
	s32 _94;
	s32 _98;
	s32 _9C;
	s32 _A0;
	s32 _A4;
	s32 _A8;
	s32 _AC;
	s32 _B0;
	s32 _B4;
	u8 _B8;
	u8 _B9;
	u8 _BA;
	Nerve* mNrvWait;
	Nerve* mNrvMove;
	Nerve* mNrvDone;
	s32 _C8;
	s32 _CC;
};
