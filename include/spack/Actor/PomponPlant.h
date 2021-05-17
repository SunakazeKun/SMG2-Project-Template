#pragma once 

#include "Actor/LiveActor/LiveActor.h"

class PomponPlant : public LiveActor {
public:
    PomponPlant(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    bool tryGenItem();
    void exeSwingDpd();
    void exeSwingPlayer();

    s32 mCollectible;
    s32 mAmount;
    bool mItemGenerated;
};

namespace NrvPomponPlant {
    class NrvWait : public Nerve {
    public:
        NrvWait() {}
        virtual void execute(Spine *) const;

        static NrvWait sInstance;
    };

    class NrvSwingDpd : public Nerve {
    public:
        NrvSwingDpd() {}
        virtual void execute(Spine *) const;

        static NrvSwingDpd sInstance;
    };

    class NrvSwingPlayer : public Nerve {
    public:
        NrvSwingPlayer() {}
        virtual void execute(Spine *) const;

        static NrvSwingPlayer sInstance;
    };
};
