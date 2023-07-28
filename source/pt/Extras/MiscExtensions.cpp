#include "syati-light.h"

/*
* Authors: Aurum
*/
namespace pt {
	/*
	* Error Message Fallback
	*
	* Missing text will usualy crash the game. This small patch will display the text label whose message data could not be
	* located instead of nothing. This tells developers what text is missing.
	*/
	static wchar_t cErrorMessageBuffer[128]; // Buffer for missing text label. Last wchar should always be NULL.

	const wchar_t* getErrorMessage(const char *pLabel) {
		asm("mr %0, r31" : "=r" (pLabel));

		size_t len = strlen(pLabel) + 1;

		if (len > 127) {
			len = 127;
		}

		mbstowcs(cErrorMessageBuffer, pLabel, len);

		return cErrorMessageBuffer;
	}

	kmCall(0x800413F0, getErrorMessage); // MR::getGameMessageDirect will return the error message instead of NULL


	///*
	//* Shell-casting Magikoopa
	//*/
	//void initNewKameck(Kameck *pActor, const JMapInfoIter &rIter) {
	//	pActor->initJMapParam(rIter);
	//
	//	if (MR::isValidInfo(rIter)) {
	//		if (MR::isObjectName(rIter, "KameckJetTurtle")) {
	//			pActor->mBeamType = KAMECK_BEAM_TURTLE;
	//		}
	//	}
	//}
	//
	//kmCall(0x801A49D4, initNewKameck);
	//
	///*
	//* While carrying over stuff from the first game, they forgot to update parts of the KameckTurtle actor. Therefore,
	//* it will crash the game and cause various other problems. First of all, it tries to load the animation from SMG1,
	//* which does not exist anymore (Koura.brk was renamed to Color.brk). Also, Mario won't try to pull in the shell
	//* due to the shell's actor name being wrong. For some reason it expects a specific actor name...
	//* Lastly, the actor should be dead by default, but they made it appear nevertheless.
	//*/
	//void initFixKameckTurtle(LiveActor *pActor) {
	//	pActor->mName = "カメックビーム用カメ";
	//	MR::startBrk(pActor, "Color");
	//}
	//
	//kmCall(0x801A8CFC, initFixKameckTurtle); // redirect BRK assignment to initialization fix
	//kmWrite32(0x801A8DD0, 0x818C0038);       // Call makeActorDead instead of makeActorAppeared
}
