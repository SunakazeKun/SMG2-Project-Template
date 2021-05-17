#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/PartsModel.h"
#include "JGeometry/TPosition3.h"

class WatchTowerRotateStep : public LiveActor {
public:
    WatchTowerRotateStep(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    void initLift(const JMapInfoIter &);
    void attachLift();

    void exeMove();
    void exeMoveStart();

    TVec3f mRotateDeg;
    PartsModel* mParts[4];
    f32 mRotSpeed;
};

namespace WatchTowerRotateStepNrv {
    class NrvMove : public Nerve {
    public:
        NrvMove() {}
        virtual void execute(Spine *) const;

        static NrvMove sInstance;
    };

    class NrvMoveStart : public Nerve {
    public:
        NrvMoveStart() {}
        virtual void execute(Spine *) const;

        static NrvMoveStart sInstance;
    };

    class NrvWait : public Nerve {
    public:
        NrvWait() {}
        virtual void execute(Spine *) const;

        static NrvWait sInstance;
    };
};
