#pragma once

#include "Actor/LiveActor/LiveActor.h"

class CrystalSwitch : public LiveActor {
public:
    CrystalSwitch(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();
    void calcRotSpeed();

    s32 mActiveTime;
    f32 mRotSpeed;
    u8 mStartSpin;
};

namespace NrvCrystalSwitch {
    class NrvOff : public Nerve {
    public:
        NrvOff() { }

        virtual void execute(Spine*) const;

        static NrvOff sInstance;
    };

    class NrvSwitchDown : public Nerve {
    public:
        NrvSwitchDown() { }

        virtual void execute(Spine*) const;

        static NrvSwitchDown sInstance;
    };

    class NrvOn : public Nerve {
    public:
        NrvOn() { }

        virtual void execute(Spine*) const;

        static NrvOn sInstance;
    };

    class NrvSwitchUp : public Nerve {
    public:
        NrvSwitchUp() { }

        virtual void execute(Spine*) const;

        static NrvSwitchUp sInstance;
    };
};
