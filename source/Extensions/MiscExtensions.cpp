#include "spack/MapObj/BlueChip.h"
#include "MapObj/Coin.h"
#include "MapObj/Shellfish.h"
#include "MapObj/SuperSpinDriver.h"
#include "Util.h"

/*
* Authors: Aurum
*/
namespace SPack {
	/*
	* Crash Debugger
	* 
	* Enables the Crash Debugger that prints valuable information whenever a crash occurs, such as
	* the stack trace and register values. This was discovered and created by Treeki and has been
	* of great help for SMG modding. You should always have this enabled in your projects/mods!
	*/
	kmWrite32(0x804B7D90, 0x60000000);
	kmWrite32(0x804B7E54, 0x60000000);
	kmWrite32(0x805B66B4, 0x60000000);

	/*
	* Green Launch Star
	* 
	* Unsurprisingly, all the BTP and BRK frames for the green Launch Star is still found inside
	* SuperSpinDriver.arc. Here, we hijack calls to SuperSpinDriver::initColor() in order to check
	* for the green color first. If the Launch Star's color is set to green, we apply its animation
	* frames. Otherwise, we call SuperSpinDriver::initColor() to set up the other colors.
	*/
	void initSuperSpinDriverGreenColor(SuperSpinDriver *pActor) {
		if (pActor->mColor == SUPER_SPIN_DRIVER_GREEN) {
			MR::startBtpAndSetFrameAndStop(pActor, "SuperSpinDriver", 1.0f);
			MR::startBrk(pActor, "Green");

			pActor->mSpinDriverPathDrawer->mColor = 0;
		}
		else
			pActor->initColor();
	}

	kmCall(0x8031E29C, initSuperSpinDriverGreenColor); // redirection hook

	/*
	* The green Launch Star is coded to load a non-opaque model from SuperSpinDriverEmpty.arc. This
	* was used in SMG1 to mark its position before all green stars are collected. However, we have no
	* use of this property in SMG2, so we can safely disable this here. This also decreases the number
	* of required memory since this model would be loaded everytime. We accomplish this by NOP-ing out
	* the call to SuperSpinDriver::initEmptyModel().
	*/
	kmWrite32(0x8031E2A4, 0x60000000);

	/*
	* New Clam items
	* 
	* Originally, I replaced the unused code for ShellfishKinokoOneUp to include my purple coin clam.
	* However, this brings up a couple of problems that I managed to fix with this setup. Nothing gets
	* replaced and new variants can be easily added and created here. As usual, I redirected the calls
	* to Shellfish::initItem(const JMapInfoIter&) to check for my new variants first. At the end, the
	* original function is called in order to initialize the other existing item types and to set up
	* the hit sensors and more.
	*/
	void initShellfishItem(Shellfish *pActor, const JMapInfoIter &rIter) {
		LiveActor* item;

		if (MR::isObjectName(rIter, "ShellfishBlueChip")) {
			s32 group = -1;
			MR::getJMapInfoArg0NoInit(rIter, &group);

			item = new BlueChip("ブルーチップ");
			((BlueChip*)item)->setHost(pActor);
			((BlueChip*)item)->setGroupID(group);
			item->initWithoutIter();

			pActor->mItem = item;
			pActor->mItemType = 2;
		}
		else if (MR::isObjectName(rIter, "ShellfishPurpleCoin")) {
			item = MR::createPurpleCoin(pActor, "パープルコイン");
			item->initWithoutIter();

			pActor->mItem = item;
			pActor->mItemType = 4;
		}
		
		pActor->initItem(rIter);
	}

	kmCall(0x802FC034, initShellfishItem); // redirection hook
	kmWrite32(0x802FC79C, 0x41820030); // skip the repetitive check for blue chips

	/*
	* Shellfish::putItem() is used to calculate the item's position relative to the clam's position.
	* In earlier versions of SPack, the purple coin clam used the offset of 1-Up mushrooms which
	* caused the model to clip through the clam. Also, the blue chip's offset loading code is still
	* part of Shellfish::putItem(), so we do not have to include that here.
	*/
	void putShellfishItem(Shellfish *pActor) {
		Mtx* mtx = pActor->getBaseMtx();
		TVec3f* offset;

		if (pActor->mItemType == 4) {
			TVec3f offset(0.0f, 50.0f, 30.0f);
			PSMTXMultVec(*mtx, (Vec*)&offset, (Vec*)&pActor->mItem->mTranslation);
		}
		else
			pActor->putItem();
	}

	kmCall(0x802FC22C, putShellfishItem); // redirection hooks
	kmCall(0x802FC838, putShellfishItem); 
	kmCall(0x802FCA58, putShellfishItem);

	/*
	* QuakeEffectGenerator fix
	* 
	* QuakeEffectGenerator never plays the earthquake sound as its sound data is never initialized.
	* This is likely an oversight from when the SMG2 developers upgraded SMG1's sound system since
	* sounds are loaded slightly different in SMG1. The fix is simple, just make it initialize the
	* sound data in the init function.
	*/
	void initQuakeEffectGeneratorSound(LiveActor *pActor) {
		MR::invalidateClipping(pActor);
		pActor->initSound(1, "QuakeEffectGenerator", false, TVec3f(0.0f, 0.0f, 0.0f));
	}

	kmCall(0x8026360C, initQuakeEffectGeneratorSound); // redirection hook
}
