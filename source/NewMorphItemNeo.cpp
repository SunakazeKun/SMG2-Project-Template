#include "spack/Actor/NewMorphItemNeo.h"
#include "Util/EffectUtil.h"
#include "Util/SoundUtil.h"

/*
* Created by Aurum
* 
* Two Power-Ups from SMG1, the Ice Flower and Flying Star, didn't make the cut when SMG2 was made.
* Luckily enough, all of their transformation code still remains in the game, the only thing that
* is missing, beside the actual models, are the collectible power-up items.
* 
* It appears that the developers attempted to delete some of the Flying Star's functionality since
* wall jumping and pulling coins does not work anymore.
*/
MorphItemNeoIce::MorphItemNeoIce(const char* pName) : MorphItemObjNeo(pName) {}

/*
* In SMG2, the MorphItemObjNeo class has a method that returns the transformation type instead of
* an attribute field. Therefore, we simply override this method and make it return the proper ID.
*/
u32 MorphItemNeoIce::getPowerUp() {
	return MORPH_ICE;
}

/*
* The Ice Flower has special effects for appearing which are still found in AutoEffectList.bcsv,
* although they are unused in the final game. Not all power-ups support this, however, so I
* forgot to implement this properly in earlier versions of SPack.
*/
void MorphItemNeoIce::emitAppearEffects() {
	MR::emitEffect(this, "AppearIceFlower");
	MR::startLevelSound(this, "OjMorphFlowerAppear", -1, -1, -1);
}

kmWrite32(0x800240C4, 0x48000010); // set first collection flag
kmWrite32(0x80024544, 0x60000000); // skip collection flag check

MorphItemNeoFoo::MorphItemNeoFoo(const char* pName) : MorphItemObjNeo(pName) {}

u32 MorphItemNeoFoo::getPowerUp() {
	return MORPH_FOO;
}

/*
* Like the Ice Flower, the Flying Star also has the previously described unused effects. These
* are properly implemented here, finally...
*/
void MorphItemNeoFoo::emitAppearEffects() {
	MR::emitEffect(this, "AppearFoo");
	MR::startLevelSound(this, "OjMorphItemAppear", -1, -1, -1);
}

kmWrite32(0x80024104, 0x48000010); // set first collection flag
kmWrite32(0x80024584, 0x60000000); // skip collection flag check
