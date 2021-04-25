#include "spack/Actor/BlueChip.h"
#include "MapObj/Coin.h"
#include "MapObj/MorphItemCollection.h"
#include "MapObj/Shellfish.h"
#include "MapObj/SuperSpinDriver.h"
#include "System/ScenarioDataParser.h"
#include "Util/JMapUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/SceneUtil.h"
#include "Util/StringUtil.h"
#include "kamek.h"
#include "mtx.h"

namespace SPack {
	/*
	* Enables the Crash debugger which prints useful information whenever a crash occurs, such as the
	* stack frame, call stack, register values and more. This was originally created by Treeki and has
	* been of great help for SMG modding. You should always have this in your projects/mods/hacks!
	*/
	kmWrite32(0x804B7D90, 0x60000000);
	kmWrite32(0x804B7E54, 0x60000000);
	kmWrite32(0x805B66B4, 0x60000000);

	/*
	* MorphItemCollection checks the object's name to detect the proper power-up type. This is done by
	* MorphItemCollection::initPowerUpModel(const JMapInfoIter&). We hijack calls to this function,
	* allowing us to check our custom object names first and apply the power-up types accordingly.
	* However, any other object name is redirected to be checked in the original function.
	*/
	void initCollectionPowerUp(MorphItemCollection* pActor, const JMapInfoIter& rIter) {
		if (MR::isObjectName(rIter, "MorphItemCollectionIce"))
			pActor->mPowerUpType = MORPH_ICE;
		else if (MR::isObjectName(rIter, "MorphItemCollectionFoo"))
			pActor->mPowerUpType = MORPH_FOO;
		else
			pActor->initPowerUpModel(rIter);
	}

	kmCall(0x802D1C3C, initCollectionPowerUp); // redirection hook

	/*
	* Luckily, all the BTP and BRK data for the green Launch Star is still present in SuperSpinDriver.arc.
	* Here, we hijack calls to SuperSpinDriver::initColor() in order to check for the green color first.
	* If the Launch Star's color is set to green, apply the color frame and animation accordingly. Else,
	* we call SuperSpinDriver::initColor() to set up the other colors since we do not have to duplicate
	* the code.
	*/
	void initSuperSpinDriverGreenColor(SuperSpinDriver* pActor) {
		if (pActor->mColor == SUPER_SPIN_DRIVER_GREEN) {
			MR::startBtp(pActor, "SuperSpinDriver");
			MR::setBtpFrameAndStop(pActor, 1.0f);
			MR::startBrk(pActor, "Green");
			pActor->mSpinDriverPathDrawer->mColor = 0;
		}
		else
			pActor->initColor();
	}

	kmCall(0x8031E29C, initSuperSpinDriverGreenColor); // redirection hook

	/*
	* The green Launch Star is coded to load a semi-transparent model from SuperSpinDriverEmpty.arc.
	* This was used in SMG1 to mark its position before all green stars are collected. However, we do
	* not need this behavior in SMG2, so we can disable this. This also decreases the number of
	* required memory since this model would be loaded anytime. Effectively, this removes the need to
	* patch in SuperSpinDriverEmpty.arc as well. We accomplish this by NOP-ing out the call to
	* SuperSpinDriver::initEmptyModel().
	*/
	kmWrite32(0x8031E2A4, 0x60000000);

	/*
	* Originally, I replaced the unused code for ShellfishKinokoOneUp to include my purple coin clam.
	* However, this brings up a couple of problems that I managed to fix with this setup. Nothing gets
	* replaced and new variants can be easily added and created here. As usual, I redirected the calls
	* to Shellfish::initItem(const JMapInfoIter&) to check for my new variants first. At the end, the
	* original function is called in order to initialize the other existing item types and to set up
	* the hit sensors and more.
	*/
	void initShellfishItem(Shellfish* pActor, const JMapInfoIter& rIter) {
		LiveActor* item;

		if (MR::isObjectName(rIter, "ShellfishBlueChip")) {
			pActor->mItemType = 2;

			s32 group = -1;
			MR::getJMapInfoArg0NoInit(rIter, &group);

			item = new BlueChip("ブルーチップ");
			((BlueChip*)item)->setHost(pActor);
			((BlueChip*)item)->setGroupID(group);
			item->initWithoutIter();
			pActor->mItem = item;
		}
		else if (MR::isObjectName(rIter, "ShellfishPurpleCoin")) {
			pActor->mItemType = 4;

			item = MR::createPurpleCoin(pActor, "パープルコイン");
			item->initWithoutIter();
			pActor->mItem = item;
		}
		
		pActor->initItem(rIter);
	}

	kmCall(0x802FC034, initShellfishItem); // redirection hook
	kmWrite32(0x802FC79C, 0x41820030); // skip the repetitive check for blue chips

	/*
	* Shellfish::putItem() is used to set the item's position relative to the clam's position. We
	* have to adjust this properly so that the items are collectable and don't clip through the
	* clam's model. In earlier versions of SPack, the purple coin clam used the offset of 1-Up
	* mushrooms. Also, the blue chip's offset loading code is still part of Shellfish::putItem(),
	* so we do not have to include that here.
	*/
	void putShellfishItem(Shellfish* pActor) {
		Mtx* mtx = pActor->getBaseMtx();
		JGeometry::TVec3<f32>* offset;

		if (pActor->mItemType == 4) {
			offset = new JGeometry::TVec3<f32>(0.0f, 50.0f, 30.0f);
			PSMTXMultVec(*mtx, offset, &pActor->mItem->mTranslation);
		}
		else
			pActor->putItem();
	}

	kmCall(0x802FC22C, putShellfishItem); // redirection hooks
	kmCall(0x802FC838, putShellfishItem); 
	kmCall(0x802FCA58, putShellfishItem);

	/*
	* QuakeEffectGenerator never plays the earthquake sound as its sound data isn't properly
	* initialized. This is an oversight from when the SMG2 developers updated SMG1's sound
	* system since sounds are loaded slightly different in SMG1. The fix is simple, however.
	*/
	void initQuakeEffectGeneratorSound(LiveActor* pActor) {
		MR::invalidateClipping(pActor);
		pActor->initSound(1, "QuakeEffectGenerator", false, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	}

	kmCall(0x8026360C, initQuakeEffectGeneratorSound);

	/*
	* This is the function to retrieve the color ID based on the specified scenario's
	* PowerStarType value. This function creates a GalaxyStatusAccessor for the specified
	* stage in order to obtain the value to be found. Following this are the color IDs and
	* their corresponding string names.
	* 0: "Normal", "Hidden", anything else
	* 1: "Bronze"
	* 2: "Green", "Legacy"
	* 3: "Red"
	* 4: nothing, this is the transparent color
	* 5: "Blue"
	*/
	s32 getPowerStarColor(const char* stage, s32 scenario) {
		GalaxyStatusAccessor* gsa = MR::makeGalaxyStatusAccessor(stage);
		const char* type;
		gsa->getScenarioString("PowerStarType", scenario, &type);

		if (MR::isEqualString(type, "Bronze"))
			return 1;
		else if (MR::isEqualString(type, "Green") || MR::isEqualString(type, "LegacyGreen"))
			return 2;
		else if (MR::isEqualString(type, "Red"))
			return 3;
		else if (MR::isEqualString(type, "Blue"))
			return 5;
		return 0;
	}

	/*
	* This function returns the color ID for the specified scenario in the current stage.
	* Essentially, this calls the above function.
	*/
	s32 getPowerStarColorCurrentStage(s32 scenario) {
		return getPowerStarColor(MR::getCurrentStageName(), scenario);
	}

	/*
	* At the given address in PowerStar::initMapToolInfo(const JMapInfoIter&), there is a
	* check which always results true, allowing us to rewrite these instructions. Here, we
	* load the color ID based on the PowerStarType setting in ScenarioData.bcsv. All the
	* checks to apply the transparent or bronze color are done later on.
	*/
	kmWrite32(0x802E01B8, 0x7C030378); // copy r0 (mPowerStarID / scenario) to r3
	kmCall(0x802E01BC, getPowerStarColorCurrentStage); // retrieve color ID
	kmWrite32(0x802E01C0, 0x907E0130); // store result (r3) in mColor

	/*
	* initMapToolInfo still is not perfect meaning that there is still a thing to take
	* proper care of. If not a Green Star, the color ID should not default to 0 (gold).
	* Instead, we want our previously loaded color to be used. This replaces "li r3, 0".
	*/
	kmWrite32(0x802E01F0, 0x807E0130); // load mColor into r3

	/*
	* This adjusts the Star Select scene to apply our new colors correctly. Basically,
	* we replaced the check for Green Stars and extended it with our colors.
	*/
	kmCall(0x802DFC14, getPowerStarColorCurrentStage); // redirection hook
	kmWrite32(0x802DFC20, 0x60000000); // NOP-out returning green all the time

	/*
	* Lastly, we still have to patch the Star return cutscene to account for the newly
	* added color. This one could possibly be improved in the future to reduce all the
	* kmWrite calls.
	*/
	kmCall(0x802DF0FC, getPowerStarColor); // redirection hook
	kmWrite32(0x802DF100, 0x7C7C1B78); // copy result from r3 to r28
	kmWrite32(0x802DF104, 0x2C1C0000); // compare r28 to 0
	kmWrite32(0x802DF108, 0x41820008); // branch to apply gold or transparent

	/*
	* Whenever a Power Star is created for a cutscene, a different set of functions is
	* used to retrieve and apply the color. This is a simple fix, however. For this, the
	* function MR::isPowerStarGreenInCurrentStageWithDeclarer(const char*, s32) has been
	* changed to return the Power Star color for the specific scenario instead of checking
	* whether the scenario contains a Green Star. This effectively changes the returned
	* value from bool to s32.
	*/
	kmCall(0x8035BAC0, getPowerStarColorCurrentStage); // redirection hook
	kmWrite32(0x802DF02C, 0x7C7D1B78); // copy result from r3 to r29
	kmWrite32(0x802DF030, 0x4800000C); // skip unnecessary instructions
}
