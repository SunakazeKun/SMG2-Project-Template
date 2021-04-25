#pragma once

#define MSG_PUSH 0x33

class LiveActor;
class HitSensor;

namespace MR
{
    HitSensor* getSensor(LiveActor *, const char *);
    void addBodyMessageSensorMapObj(LiveActor *);
    HitSensor* addHitSensorMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    HitSensor* addHitSensorMapObjMoveCollision(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addBodyMessageSensor(LiveActor *, u32);

    HitSensor* addHitSensorEnemy(LiveActor*, const char*, u16, f32, const JGeometry::TVec3<f32>&);
    HitSensor* addHitSensorEnemyAttack(LiveActor*, const char*, u16, f32, const JGeometry::TVec3<f32>&);
    HitSensor* addHitSensorAtJointEnemy(LiveActor*, const char*, const char*, u16, f32, const JGeometry::TVec3<f32>&);
    HitSensor* addHitSensorMtx(LiveActor*, const char*, u32, u16, f32, Mtx4*, const JGeometry::TVec3<f32>&);
    HitSensor* addHitSensorMtxEnemy(LiveActor*, const char*, u16, f32, Mtx4*, const JGeometry::TVec3<f32>&);
    HitSensor* addHitSensorPush(LiveActor*, const char*, u16, f32, const JGeometry::TVec3<f32>&);

    void addHitSensor(LiveActor *, const char *, u32, u16, float, const JGeometry::TVec3<f32> &);

    void validateHitSensor(LiveActor*, const char*);
    void invalidateHitSensor(LiveActor*, const char*);
    void invalidateHitSensors(LiveActor*);

    bool isSensorPlayerOrRide(const HitSensor*);
    bool isSensorPlayer(const HitSensor *);
    bool isSensorEnemy(const HitSensor*);
    bool isSensorMapObj(const HitSensor*);
    void sendMsgPush(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttack(HitSensor*, HitSensor*);
    bool sendMsgEnemyAttackStrong(HitSensor*, HitSensor*);
    void sendMsgEnemyAttackExplosion(HitSensor*, HitSensor*);
    void sendMsgToGroupMember(u32, LiveActor*, HitSensor*, const char*);

    bool isMsgPlayerHitAll(u32);
    bool isMsgPlayerUpperPunch(u32);
    bool isMsgPlayerSpinAttack(u32);
    bool isMsgStarPieceAttack(u32);
    bool isMsgStarPieceReflect(u32);
    bool isMsgPlayerHipDrop(u32);
    bool isMsgPlayerHipDropFloor(u32);
    bool isMsgExplosionAttack(u32);
    bool isMsgJetTurtleAttack(u32);

    bool isMsgItemGet(u32);

    bool isMsgPlayerTrample(u32);
};