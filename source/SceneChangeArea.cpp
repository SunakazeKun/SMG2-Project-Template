#include "AreaObj/AreaObj.h"
#include "Util/PlayerUtil.h"
#include "Util/ScreenUtil.h"
#include "Util/StageUtil.h"

const char* pStages[2] = { "IslandFleetGalaxy", "RedBlueExGalaxy" };

void SceneChangeAreaControl(AreaObj *pArea)
{
    TVec3f* pos = MR::getPlayerPos();

    if (pArea->isInVolume(*pos))
    {
        MR::closeSystemWipeFadeWithCaptureScreen(pArea->mObjArg1 == -1 ? 0 : pArea->mObjArg1);
        
        if (pArea->mObjArg0 != -1)
            MR::goToGalaxy(pStages[pArea->mObjArg0]);
        else
            MR::goToGalaxy("IslandFleetGalaxy");

        pArea->mValidate = 0;
    }
}

#ifdef USA
kmWritePointer(0x8066AE4C, SceneChangeAreaControl);
#endif

#ifdef PAL
kmWritePointer(0x8067054C, SceneChangeAreaControl);
#endif

#ifdef JAP
kmWritePointer(0x8066A62C, SceneChangeAreaControl);
#endif

#ifdef TWN
kmWritePointer(0x8066B0EC, SceneChangeAreaControl);
#endif

#ifdef KOR
kmWritePointer(0x80669C8C, SceneChangeAreaControl);
#endif
