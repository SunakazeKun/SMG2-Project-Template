#pragma once

#include "Actor/Animation/ActorAnimKeeper.h"
#include "Actor/Camera/ActorPadAndCameraCtrl.h"
#include "Actor/Effect/EffectKeeper.h"
#include "Actor/Light/ActorLightCtrl.h"
#include "Actor/NameObj/NameObj.h"
#include "Actor/Nerve/Spine.h"
#include "Actor/Rail/RailRider.h"
#include "Actor/Sensor/HitSensor.h"
#include "Actor/Sensor/HitSensorKeeper.h"
#include "Actor/Shadow/ShadowController.h"
#include "Actor/Sound/AudAnmSoundObject.h"
#include "Actor/StarPointer/StarPointerTarget.h"
#include "Actor/Switch/StageSwitch.h"
#include "Collision/Binder.h"
#include "JGeometry/TVec3.h"
#include "Actor/Model/ModelManager.h"

class LiveActorFlag
{
public:
    LiveActorFlag();

    bool mIsDead; // _0
    bool mIsNotReleasedAnimFrame; // _1
    bool mIsOnCalcAnim; // _2
    bool mIsNoCalcView; // _3
    bool mIsNoEntryDrawBuffer; // _4
    bool mIsOnBind; // _5
    bool mIsCalcGravity; // _6
    bool mIsClipped; // _7
    bool mIsClippingInvalid; // _8
};

class LiveActor : public NameObj
{
public:
    LiveActor(const char *);
    ~LiveActor();

    virtual void init(JMapInfoIter const &);

    virtual void movement();

    virtual void calcAnim();
    virtual void calcViewAndEntry();

    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual s32 receiveMessage(u32, HitSensor *, HitSensor *);
    virtual Mtx* getBaseMtx() const;
    virtual Mtx* getTakingMtx() const;
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual u32 receiveMsgPush(HitSensor *, HitSensor *);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initStageSwitch(const JMapInfoIter &iter);
    void initRailRider(const JMapInfoIter &);
    void initHitSensor(s32);
    void initModelManagerWithAnm(const char *, const char *);
    void initEffectKeeper(s32, const char *, bool);
    void initSound(s32, const char *, bool, const JGeometry::TVec3<f32> &);
    void initNerve(const Nerve *, s32);
    void initShadowControllerList(u32);
    void initBinder(f32, f32, u32);

    bool isNerve(const Nerve *) const;
    void setNerve(const Nerve *);

    s32 getNerveStep() const;

    void func_8000C170(const JMapInfoIter &, const char *, bool);

    HitSensor* getSensor(const char *) const;

    JGeometry::TVec3<f32> mTranslation; // _14
    JGeometry::TVec3<f32> mRotation; // _20
    JGeometry::TVec3<f32> mScale; // _2C
    JGeometry::TVec3<f32> mVelocity; // _38
    JGeometry::TVec3<f32> mGravity; // _44
    ModelManager* mModelManager; // _50
    ActorAnimKeeper* mAnimKeeper; // _54
    Spine* mSpine; // _58
    HitSensorKeeper* mSensorKeeper; //_5C
    Binder* mBinder; // _60
    RailRider* mRailRider; // _64
    EffectKeeper* mEffectKeeper; // _68
    AudAnmSoundObject* mSoundObj; // _6C
    u32 _70;
    u32 _74;
    //LiveActorFlag mFlags; // _70
    ShadowControllerList* mShadowController; // _78
    u32* _7C; // CollisionParts*
    StageSwitchCtrl* mStageSwitchCtrl; // _80
    StarPointerTarget* mPointerTarget; // _84
    ActorLightCtrl* mLightCtrl; // _88
    ActorPadAndCameraCtrl* mCameraCtrl; // _8C
};