#pragma once

#include "LiveActor.h"

class Jiraira : public LiveActor {
public:
    Jiraira(const char *pName);

    virtual void init(const JMapInfoIter &rIter);
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

    void exeWait();
    void exeStepped();
    void exeExplode();
    void exePreRecover();
    void exeRecover();

    CollisionParts* mButtonCollision;
    f32 mExplosionRange;
};

namespace NrvJiraira {
    NERVE(NrvWait);
    NERVE(NrvStepped);
    NERVE(NrvPreRecover);
    NERVE(NrvRecover);
    NERVE(NrvSteppedExplode);
    NERVE(NrvExplode);
};
