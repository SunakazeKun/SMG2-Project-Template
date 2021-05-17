#pragma once 

#include "Actor/LiveActor/LiveActor.h"
#include "MapObj/MapObjConnector.h"

class ScrewSwitchBase : public LiveActor {
public:
    ScrewSwitchBase(const char*);

    virtual void initAfterPlacement();
    virtual void calcAndSetBaseMtx();
    virtual u32 receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void exeWait() = 0;
    virtual void exeAdjust() = 0;
    virtual void exeScrew() = 0;
    virtual void exeEnd();

    void updateBindActorMtx();

    LiveActor* mBindedActor;
    MapObjConnector* mMapObjConnector;
};

namespace NrvScrewSwitchBase {
    class NrvWait : public Nerve {
    public:
        NrvWait() {}
        virtual void execute(Spine *) const;

        static NrvWait sInstance;
    };

    class NrvAdjust : public Nerve {
    public:
        NrvAdjust() {}
        virtual void execute(Spine *) const;

        static NrvAdjust sInstance;
    };

    class NrvScrew : public Nerve {
    public:
        NrvScrew() {}
        virtual void execute(Spine *) const;

        static NrvScrew sInstance;
    };

    class NrvEnd : public Nerve {
    public:
        NrvEnd() {}
        virtual void execute(Spine *) const;

        static NrvEnd sInstance;
    };
};

class ScrewSwitch : public ScrewSwitchBase {
public:
    ScrewSwitch(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void exeWait();
    virtual void exeAdjust();
    virtual void exeScrew();

    CollisionParts* mScrewCollision;
};

class ScrewSwitchReverse : public ScrewSwitchBase {
public:
    ScrewSwitchReverse(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void exeWait();
    virtual void exeAdjust();
    virtual void exeScrew();

    void endBind();

    bool mForceJump;
};

class ValveSwitch : public ScrewSwitchBase {
public:
    ValveSwitch(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void exeWait();
    virtual void exeAdjust();
    virtual void exeScrew();

    Mtx mCollisionMtx;
    bool mIsReverse;
};
