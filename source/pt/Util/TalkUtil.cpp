#include "pt/Util/TalkUtil.h"

/*
* Author: Aurum
*/

namespace pt {
	void setMessageArg(TalkMessageCtrl *pTalkCtrl, const wchar_t *pText) {
		CustomTagArg tagArg;
		tagArg.mType = 1;
		tagArg.mStringArg = pText;
		pTalkCtrl->setMessageArg(tagArg);
	}
}
