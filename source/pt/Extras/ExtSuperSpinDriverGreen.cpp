#include "pt/Extras/ExtSuperSpinDriverGreen.h"
#include "Game/MapObj/SuperSpinDriver.h"
#include "Game/Util/ActorAnimUtil.h"

/*
* Author: Aurum
*/

namespace pt {
	namespace {
		/*
		* Unsurprisingly, all the BTP and BRK frames for the green Launch Star is still found inside SuperSpinDriver.arc. Here,
		* we hijack calls to initColor in order to check for the green color first. If the Launch Star's color is set to green,
		* we apply its animation frames. Otherwise, we call initColor to set up the other colors.
		*/
		void initSuperSpinDriverGreenColor(SuperSpinDriver *pActor) {
			if (pActor->mColor == SUPER_SPIN_DRIVER_GREEN) {
				MR::startBtpAndSetFrameAndStop(pActor, "SuperSpinDriver", 1.0f);
				MR::startBrk(pActor, "Green");

				pActor->mSpinDriverPathDrawer->mColor = 0;
			}
			else {
				pActor->initColor();
			}
		}

		kmCall(0x8031E29C, initSuperSpinDriverGreenColor); // redirect initColor in init

		/*
		* The Green Launch Star is coded to load a model from SuperSpinDriverEmpty.arc. This was used for the transparent model
		* in SMG1 to mark its position before all green stars are collected. However, we have no use of this property in SMG2,
		* so we can safely disable this here. This also improves memory usage since this model would be loaded at all times.
		*/
		kmWrite32(0x8031E2A4, 0x60000000); // NOP call to initEmptyModel.
	}

	NameObj* createSuperSpinDriverGreen(const char *pName) {
		return new SuperSpinDriver(pName, SUPER_SPIN_DRIVER_GREEN);
	}
}
