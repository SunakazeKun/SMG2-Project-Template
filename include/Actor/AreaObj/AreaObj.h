#pragma once

#include "Actor/AreaObj/AreaFormCube.h"
#include "Actor/NameObj/NameObj.h"
#include "JGeometry/TVec3.h"

class AreaObj : public NameObj
{
public:
    AreaObj(const char *);
    AreaObj(s32, const char *);

    virtual ~AreaObj();
    virtual void init(const JMapInfoIter &);

    virtual bool isInVolume(const JGeometry::TVec3<f32> &) const;
    virtual s32 getAreaPriority() const;
    virtual const char* getManagerName() const;

    void onSwitchA();
    void offSwitchA();
    bool isOnSwitchA() const;

    s32 mType; // _14
    u32 _18;
    u8 mValidate; // _1C
    u8 mFollowActorAlive; // _1D
    u16 _1E;
    s32 mObjArg0; // 20
    s32 mObjArg1; // _24
    s32 mObjArg2; // _28
    s32 mObjArg3; // _2C
    s32 mObjArg4; // _30
    s32 mObjArg5; // _34
    s32 mObjArg6; // _38
    s32 mObjArg7; // _3C
    s32 mPriority; // _40
    u32* mSwitchCtrl; // _44 (StageSwitchCtrl*)
};