#pragma once

#include "JGeometry/TVec3.h"

class NameObj;
class LiveActor;

namespace MR
{
    void connectToSceneMapObj(LiveActor *);
    void connectToSceneMapObjMovement(NameObj *);
    void connectToSceneMapParts(LiveActor*);
    void connectToSceneEnemy(LiveActor *);
    void connectToSceneCollisionEnemy(LiveActor*);
    void connectToSceneEnemyMovement(NameObj *);
    void connectToSceneNpc(LiveActor *);
    void connectToSceneAreaObj(NameObj*);

    void connectToSceneItemStrongLight(LiveActor*);

    void connectToSceneNoSilhouettedMapObjStrongLight(LiveActor*);

    void connectToSceneMapObjNoMovement(LiveActor *);

    void connectToSceneCollisionMapObj(LiveActor *);
    void connectToSceneCollisionMapObjStrongLight(LiveActor*);

    void registerNameObjToExecuteHolder(NameObj *, int, int, int, int);

    bool isStageStateScenarioOpeningCamera();

    bool tryRumblePadMiddle(const void *, s32);
    bool tryRumblePadWeak(const void *, s32);

    void shakeCameraVeryStrong();
    void shakeCameraStrong();
    void shakeCameraNormalStrong();
    void shakeCameraNormal();
    void shakeCameraNormalWeak();
    void shakeCameraWeak();
    void shakeCameraVeryWeak();

    void declarePowerStar(const NameObj *);
    void declarePowerStar(const NameObj *, s32);
    void requestAppearPowerStar(const NameObj *, const JGeometry::TVec3<f32>&);
    void requestAppearPowerStar(const NameObj*, s32, const JGeometry::TVec3<f32>&);

    void declareStarPiece(const NameObj *, s32);
    bool appearStarPiece(const NameObj *, const JGeometry::TVec3<f32> &, s32, f32, f32, bool);

    void declareCoin(const NameObj*, s32);
    bool appearCoinPop(const NameObj*, const JGeometry::TVec3<f32>&, s32);
};