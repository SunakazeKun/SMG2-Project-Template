#include "spack/Extensions/PowerStarColors.h"
#include "System/GalaxyStatusAccessor.h"
#include "System/ScenarioDataParser.h"
#include "Util.h"
#include "spack/Util/ActorUtil.h"
#include "LiveActor/LiveActor.h"

/*
* Authors: Aurum
* 
* This changes all the code that apply Power Star colors to support new and custom colors as well.
* To accomplish this, the PowerStarType in the scenario file is changed to configure the color for
* a scenario's star easily. The new types are Bronze, Red, Blue and LegacyGreen. As the Green type
* from the base game has to be unlocked first, the LegacyGreen type was added in to ignore these
* conditions effectively.
*/
namespace SPack {
	/*
	* This is the function to retrieve the color ID based on the specified scenario's
	* PowerStarType value. This function creates a GalaxyStatusAccessor for the specified
	* stage in order to obtain the value to be found. Following this are the color IDs and
	* their corresponding string names.
	* 0: "Normal", "Hidden", anything else
	* 1: "Bronze"
	* 2: "Green", "LegacyGreen"
	* 3: "Red"
	* 4: nothing, this is the transparent color
	* 5: "Blue"
	*/
	s32 getPowerStarColor(const char * pStage, s32 scenarioId) {
		const char* type;
		GalaxyStatusAccessor gsa(MR::makeGalaxyStatusAccessor(pStage));
		gsa.mScenarioData->getScenarioString("PowerStarType", scenarioId, &type);

		if (MR::isEqualSubString(type, "Bronze"))
			return 1;
		else if (MR::isEqualSubString(type, "Green") || MR::isEqualSubString(type, "LegacyGreen"))
			return 2;
		else if (MR::isEqualSubString(type, "Red"))
			return 3;
		else if (MR::isEqualSubString(type, "Blue"))
			return 5;
			
		return 0;
	}

	/*
	* This function returns the color ID for the specified scenario in the current stage.
	* Essentially, this calls the above function.
	*/
	s32 getPowerStarColorCurrentStage(s32 scenarioId) {
		return getPowerStarColor(MR::getCurrentStageName(), scenarioId);
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

	kmWrite32(0x804CB8BC, 0x48169D65); //This uses strstr instead of MR::isEqualString in the isPowerStarTypeHidden__12ScenarioDataCFl function. Allows types like BlueHidden to work.

	/*
	* Power Star Font Icons
	*
	* On the World Map and Star List, the game displays star icons based on the Power Star type and Comet.
	* However, we've changed this so that new and custom star types can be displayed.
	*
	* Here we load a custom BRFNT from PTSystemData.arc so we do not have to edit the font in all languages.
    *
	* When the game selects which icon to display, it calls MR::addPictureFontCode and r4 is used to deter-
	* mine which icon from the BRFNT should be used. Here we use that to check which icon type to use, then run a check for Power Star color. 
	*/

	void loadPTPictureFont() {
		loadArcAndFile("/SystemData/PTSystemData.arc", "/Font/PTPictureFont.brfnt");
}

	kmCall(0x804B8048, loadPTPictureFont);

	wchar_t* getStarIcon(wchar_t* unk, s32 type) {
		const char *stage;
		s32 scenarioId;
		s32 icon;

		asm("mr %0, r27" : "=r" (stage));
		asm("mr %0, r31" : "=r" (scenarioId));

		s32 getStarColor = getPowerStarColor(stage, scenarioId);

		if (type == 0x37) //Normal Star icons
		switch (getStarColor) {
			case 0:
				icon = 0x37; //Normal
			break;
			case 1:
				icon = 0x72; //Bronze
			break;
			case 2:
				icon = 0x80; //LegacyGreen
			break;
			case 3:
				icon = 0x7E; //Red
			break;
			case 5:
				icon = 0x7F; //Blue
			break;
			}

		else if (type == 0x65) //Comet Star icons
			switch (getStarColor) {
			case 0:
				icon = 0x65; //Normal
			break;
			case 1:
				icon = 0x7D; //Bronze
			break;
			case 2:
				icon = 0x4F; //LegacyGreen
			break;
			case 3:
				icon = 0x81; //Red
			break;
			case 5:
				icon = 0x82; //Blue
			break;
			}

		else if (type == 0x71) //Uncollected Hidden Star icons
		switch (getStarColor) {
			case 0:
				icon = 0x71; //Normal
			break;
			case 1:
				icon = 0x86; //Bronze
			break;
			case 2:
				icon = 0x85; //LegacyGreen
			break;
			case 3:
				icon = 0x83; //Red
			break;
			case 5:
				icon = 0x84; //Blue
			break;
			}
		return MR::addPictureFontCode(unk, icon);
	}
	
	kmCall(0x80041E30, getStarIcon); //Normal Star icons
	kmCall(0x80041F0C, getStarIcon); //Comet Star icons
	kmCall(0x80041F94, getStarIcon); //Hidden Star icons
	kmCall(0x80041F48, getStarIcon); //Collected Hidden Star icons

	/*
	*	Star Ball: Custom Ball and Star Colors
	*
	*	The Power Star inside the Star Ball is not a display model.
	*	Here we set the Star and Ball's color by checking the color
	*	of the Scenario specified by obj_arg1.
	*/

	void TamakoroCustomPowerStarColors(LiveActor* actor, const JMapInfoIter& iter) {
		s32 argScenario = -1;

		MR::getJMapInfoArg1NoInit(iter, &argScenario);

		s32 colorFrame = SPack::getPowerStarColorCurrentStage(argScenario);

		MR::startBtpAndSetFrameAndStop(actor, "BallStarColor", colorFrame);
		MR::startBrkAndSetFrameAndStop(actor, "BallColor", colorFrame);

		if (MR::hasPowerStarInCurrentStage(argScenario))
			MR::startBrkAndSetFrameAndStop(actor, "BallColor", 4);
	}

	kmWrite32(0x8044461C, 0x7F84E378);
	kmCall(0x80444620, TamakoroCustomPowerStarColors);
};