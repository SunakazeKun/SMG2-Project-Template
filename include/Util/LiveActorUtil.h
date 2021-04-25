#pragma once

#include "JGeometry/TVec3.h"
#include "JGeometry/TPosition3.h"
#include "JGeometry/TMatrix34.h"
#include "JMap/JMapInfoIter.h"

#include "kamek.h"

class LiveActor;
class HitSensor;
class BaseMatrixFollowValidater;
class PartsModel;
class BckControl;

namespace MR
{
    void validateClipping(LiveActor *);
    void invalidateClipping(LiveActor *);

    void setPosition(LiveActor *, const JGeometry::TVec3<f32> &);

    void initLightCtrl(LiveActor*);
    void initCollisionParts(LiveActor *, const char *, HitSensor *, Mtx4 *);

    void setBaseTRMtx(LiveActor *, Mtx);
    void setBaseTRMtx(LiveActor *, const JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > &);

    void initDefaultPos(LiveActor *, const JMapInfoIter &);

    void setClippingTypeSphere(LiveActor *, f32);

    void setClippingFarMax(LiveActor *);

    void setClippingTypeSphereContainsModelBoundingBox(LiveActor *, f32);

    bool isDead(const LiveActor *);

    bool isOnGround(const LiveActor *);
    bool isBindedGround(const LiveActor *);

    void calcGravity(LiveActor *, const JGeometry::TVec3<f32> &);
    void onCalcGravity(LiveActor*);
    void offCalcGravity(LiveActor*);

    JGeometry::TVec3<f32>* getGroundNormal(const LiveActor *);

    void startBck(const LiveActor *, const char *);
    void startBckWithInterpole(const LiveActor*, const char*, s32);
    bool tryStartBck(const LiveActor *, const char *, const char *);
    void stopBck(const LiveActor *);
    bool isBckStopped(const LiveActor *);

    void startBpk(const LiveActor*, const char*);
    void startBtp(const LiveActor*, const char*);
    void startBrk(const LiveActor*, const char*);
    void setBckFrame(const LiveActor*, f32);
    void setBtpFrameAndStop(const LiveActor*, f32);

    void setBckRate(const LiveActor *, f32);

    bool isActionEnd(const LiveActor *);

    bool isFirstStep(const LiveActor *);
    bool isStep(const LiveActor *, s32);
    bool isGreaterStep(const LiveActor *, s32);
    bool isGreaterEqualStep(const LiveActor*, s32);
    bool isLessStep(const LiveActor*, s32);
    f32 calcNerveValue(const LiveActor*, s32, f32, f32);

    void invalidateCollisionParts(LiveActor *);
    void validateCollisionParts(LiveActor *);

    void addToAttributeGroupSearchTurtle(const LiveActor *);

    void copyTransRotateScale(const LiveActor*, LiveActor*);
    void setGroupClipping(LiveActor*, const JMapInfoIter&, int);

    void showModel(LiveActor*);
    void hideModel(LiveActor*);

    void addBaseMatrixFollowTarget(LiveActor*, const JMapInfoIter&, const JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >*, BaseMatrixFollowValidater*);
    PartsModel* createPartsModelNoSilhouettedMapObj(LiveActor*, const char*, const char*, Mtx4*);

    void calcAnimDirect(LiveActor*);
    void joinToGroupArray(LiveActor*, const JMapInfoIter&, const char*, s32);

    BckControl* getBckCtrl(const LiveActor*);
    f32 getBckFrame(const LiveActor*);

    void setBaseScale(LiveActor*, const JGeometry::TVec3<f32>&);

    void tryRumblePadAndCameraDistanceStrong(const LiveActor*, f32, f32, f32);
};