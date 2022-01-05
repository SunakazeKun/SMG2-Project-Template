#pragma once 

#include "LiveActor.h"

class SwitchBox : public LiveActor {
public:
    SwitchBox(const char *);

    virtual void init(const JMapInfoIter &);
	virtual bool receiveMessage(u32, HitSensor *, HitSensor *);

    void exeOn();
    void exeReturn();
    void exe2P();

    s32 mTimer;
    bool mUseRespawn;
    bool mUseTimerSe;
};

namespace NrvSwitchBox {
    class NrvWait : public Nerve {
    public:
        NrvWait() { }

        virtual void execute(Spine *) const;

        static NrvWait sInstance;
    };

    class NrvBreak : public Nerve {
    public:
        NrvBreak() { }

        virtual void execute(Spine *) const;

        static NrvBreak sInstance;
    };

    class NrvReturn : public Nerve {
    public:
        NrvReturn() { }

        virtual void execute(Spine *) const;

        static NrvReturn sInstance;
    };
}