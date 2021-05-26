#include "MapObj/ItemDice.h"

class SwitchDice : public DiceBase {
public:
	SwitchDice(const char* pName) : DiceBase(pName) {}

	virtual const char* getModelName();
	virtual void initItems();
	virtual void setAnimFrame();
	virtual void setResultFrame(u32);
	virtual bool makeResult(u32);
	virtual bool isGoodResult(u32);
};
