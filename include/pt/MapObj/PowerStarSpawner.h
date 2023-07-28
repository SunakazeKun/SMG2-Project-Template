#pragma once

#include "syati.h"

namespace pt {
    class PowerStarSpawner : public LiveActor {
    public:
        PowerStarSpawner(const char *pName);

        virtual void init(const JMapInfoIter &rIter);

        void startSpawn();
        void exePrepare();
        void exeSpawn();

        s32 mScenario;
        s32 mDelay;
        bool mFromMario;
        bool mPlayFanfare;
    };

    namespace NrvPowerStarSpawner {
        NERVE(NrvWait);
        NERVE(NrvPrepare);
        NERVE(NrvSpawn);
    };
};

#define NrvPowerStarSpawnerWait &pt::NrvPowerStarSpawner::NrvWait::sInstance
#define NrvPowerStarSpawnerPrepare &pt::NrvPowerStarSpawner::NrvPrepare::sInstance
#define NrvPowerStarSpawnerSpawn &pt::NrvPowerStarSpawner::NrvSpawn::sInstance
