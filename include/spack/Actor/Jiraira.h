#pragma once

#include "Actor/LiveActor/LiveActor.h"

class Jiraira : public LiveActor {
public:
    Jiraira(const char*);

    virtual void init(const JMapInfoIter& Iter);
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual u32 receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeStepped();
    void exeExplode();
    void exePreRecover();
    void exeRecover();

    f32 mExplosionRange;
};

namespace NrvJiraira {
    class NrvWait : public Nerve {
    public:
        NrvWait() {}

        virtual void execute(Spine*) const;

        static NrvWait sInstance;
    };

    class NrvStepped : public Nerve {
    public:
        NrvStepped() {}

        virtual void execute(Spine*) const;

        static NrvStepped sInstance;
    };

    class NrvExplode : public Nerve {
    public:
        NrvExplode() {}

        virtual void execute(Spine*) const;

        static NrvExplode sInstance;
    };

    class NrvPreRecover : public Nerve {
    public:
        NrvPreRecover() {}

        virtual void execute(Spine*) const;

        static NrvPreRecover sInstance;
    };

    class NrvRecover : public Nerve {
    public:
        NrvRecover() {}

        virtual void execute(Spine*) const;

        static NrvRecover sInstance;
    };
};
