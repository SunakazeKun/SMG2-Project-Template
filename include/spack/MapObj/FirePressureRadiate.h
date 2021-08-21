#pragma once

#include "LiveActor.h"
#include "Util/JointController.h"

class FirePressureRadiate : public LiveActor {
public:
	FirePressureRadiate(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void initAfterPlacement();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void updateHitSensor(HitSensor *pSensor);
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void exeRelax();
	void exeSyncWait();
	void exeWait();
	void exePrepareToRadiate();
	void exeRadiate();
	void exeRadiateMargin();
	void startWait();
	void startRelax();
	bool calcJointCannon(TPositionMtx *pJointMtx, const JointControllerInfo &rJointControllerInfo);

	JointController* mJointCtrl;
	Mtx mRadiateEffectMtx;
	f32 mCannonRotation;
	s32 mWaitTime;
	s32 mRadiateTime;
	s32 mRadiateSteps;
	f32 mFlameLength;
	f32 mFlameLengthMax;
	MsgSharedGroup* mMsgGroup;
	bool mGroupLeader;
};

namespace NrvFirePressureRadiate {
	NERVE(NrvRelax);
	NERVE(NrvSyncWait);
	NERVE(NrvWait);
	NERVE(NrvPrepareToRadiate);
	NERVE(NrvRadiate);
	NERVE(NrvRadiateMargin);
};
