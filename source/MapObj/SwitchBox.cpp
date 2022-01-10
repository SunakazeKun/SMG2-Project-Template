#include "spack/MapObj/SwitchBox.h"
#include "Util.h"

/*
 One of the older SMG2PT objects.
 SwitchBox's development was started by me after PTimerSwitch was finished.
 SwitchBox was originally scrapped due to the particle BTI images being removed from the game.
 I used PyJPC, a really old particle tool, to add the SMG1 JPA files into 2, which is easy.
 Though it could not add BTI images without destroying the game's existing particles.
 However, Aurum made pygapa, a tool which makes particle adding easy.

 And so, this is a rewrite of SMG1's SwitchBox added into 2 with even more options.
*/

SwitchBox::SwitchBox(const char* pName) : LiveActor(pName) {
	mTimer = 0;
	mUseRespawn = false;
	mUseTimerSe = false;
}

void SwitchBox::init(const JMapInfoIter& rIter) {
	MR::processInitFunction(this, rIter, false);
	initHitSensor(1); //Initializes the HitSensor.

	MR::addHitSensorMapObj(this, "SwitchBox", 1, 75.0f, TVec3f(0.0f, 150.0f, 0.0f));
	HitSensor * SwitchBox = LiveActor::getSensor("SwitchBox");

	MR::getJMapInfoArg0NoInit(rIter, &mTimer); //Gets Obj_arg0
	MR::getJMapInfoArg1NoInit(rIter, &mUseRespawn); //Gets Obj_arg1
	MR::getJMapInfoArg2NoInit(rIter, &mUseTimerSe); //Gets Obj_arg2
	MR::validateCollisionParts(this); //Validates collision

	initNerve(&NrvSwitchBox::NrvWait::sInstance, 0); //Sets nerve state to Wait
	makeActorAppeared(); //Makes the object appear
}

void SwitchBox::exeOn() {
	if (MR::isValidSwitchDead(this)) { //Waits for 3 frames after activation and checks if the SW_DEAD was set correctly.
		MR::onSwitchDead(this); //Activates SW_DEAD.
		MR::hideModel(this); //Makes the SwitchBox invisible.
		MR::invalidateCollisionParts(this); //Makes the SwitchBox intangible.
		MR::invalidateHitSensor(this, "SwitchBox"); //Invalidates all HitSensors.
		MR::invalidateClipping(this); //Makes the SwitchBox never unload when activated.

	if (MR::isInWater(mTranslation)) { //Check if the object is placed in water.
		MR::emitEffect(this, "BreakWater"); //Display water particles and sound.
		MR::startLevelSound(this, "OjSBoxBreakWater", -1, -1, -1);
	}
	else {
		MR::emitEffect(this, "Break"); //Display non-water particles and sound.
		MR::startLevelSound(this, "OjSBoxBreak", -1, -1, -1);
	}

	if (mTimer >= 1 && mUseTimerSe == 1)
	initNerve(&NrvSwitchBox::NrvReturn::sInstance, 0); //Initalizes nerve "Return".
	}
}

void SwitchBox::exeReturn() { //This function creates a timer sound if Obj_arg 1 and Obj_arg2 are set.

	s32 step = getNerveStep();

	if (step <= mTimer) {
		if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
			if (mTimer == step)
				MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
			else if ((step % 60) == 0) {
				if (step >= (mTimer - 120))
					MR::startSystemSE("SE_SY_TIMER_A_1", -1, -1);
				else
					MR::startSystemSE("SE_SY_TIMER_A_2", -1, -1);
			}
		}
	}

	if (MR::isStep(this, mTimer)) { //Checks if the set time has run out.
		MR::offSwitchDead(this); //Deactivates SW_DEAD.

	if (mUseRespawn == 1) {
		MR::showModel(this); //Makes the SwitchBox visible.
		MR::validateCollisionParts(this); //Makes the SwitchBox tangible.
		MR::validateHitSensor(this, "SwitchBox"); //Validates all HitSensors.
		MR::emitEffect(this, "Return"); //Displays a particle effect.
		MR::validateClipping(this); //Allows the SwitchBox to be unloaded when deactive.

		initNerve(&NrvSwitchBox::NrvWait::sInstance, 0); //Sets the nerve to "Wait".
		}
	}
}

bool SwitchBox::receiveMessage(u32 msg, HitSensor *, HitSensor *)
{
	if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerHitAll(msg)) //Checks to see if the HitSensor has recieved a message,
		initNerve(&NrvSwitchBox::NrvBreak::sInstance, 0); //Sets the nerve to "Break".
		return false;
}

void SwitchBox::exe2P() {
	MR::attachSupportTicoToTarget(this); //Allows P2 to target the SwitchBox.
}

namespace NrvSwitchBox {
	void NrvWait::execute(Spine* pSpine) const {
		SwitchBox* pActor = (SwitchBox*)pSpine->mExecutor;
		pActor->exe2P();
	}

	void NrvBreak::execute(Spine* pSpine) const {
		SwitchBox* pActor = (SwitchBox*)pSpine->mExecutor;
		pActor->exeOn();
	}

	void NrvReturn::execute(Spine* pSpine) const {
		SwitchBox* pActor = (SwitchBox*)pSpine->mExecutor;
		pActor->exeReturn();
	}

	NrvWait(NrvWait::sInstance);
	NrvBreak(NrvBreak::sInstance);
	NrvReturn(NrvReturn::sInstance);
}