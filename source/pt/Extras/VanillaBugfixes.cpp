#include "syati.h"

/*
* Authors: Aurum
*/
namespace pt {
    namespace {
        /*****************************************************************************************************************/
        /* Allow Flying power-up to Wall Jump                                                                            */
        /*****************************************************************************************************************/
        kmWrite32(0x803AF520, 0x2C030000);
        kmWrite32(0x803AF524, 0x4182000C);


        /*****************************************************************************************************************/
        /* Animate Rainbow power-up properly                                                                             */
        /*****************************************************************************************************************/
        void invokeInvincibleMarioModelCalcAnim(ModelHolder *pModelHolder) {
            MR::onCalcAnim(pModelHolder);
            pModelHolder->calcAnim();
        }

        kmCall(0x803C4A0C, invokeInvincibleMarioModelCalcAnim);


        /*****************************************************************************************************************/
        /* Silver Stars replenish Air                                                                                    */
        /*****************************************************************************************************************/
        void strayTicoFillAirAndRumblePad(LiveActor *pStrayTico, s32 rumbleArg) {
            MR::incPlayerOxygen(8);
            MR::tryRumblePadMiddle(pStrayTico, rumbleArg);
        }

        kmCall(0x8035FE68, strayTicoFillAirAndRumblePad);


        /*****************************************************************************************************************/
        /* Timer Clocks can be collected while player is "riding" an object                                              */
        /*****************************************************************************************************************/
        bool checkTimeAttackClockSensor(const HitSensor *pSensor) {
            return MR::isSensorPlayerOrRide(pSensor);
        }

        kmCall(0x80323544, checkTimeAttackClockSensor);


        /*****************************************************************************************************************/
        /* Safely read KeySwitch's Obj_arg0 when constructed via othr objects.                                           */
        /*****************************************************************************************************************/
        void initKeySwitchSafeGetShadowDropLength(const JMapInfoIter &rIter, f32 *pDest) {
            if (MR::isValidInfo(rIter)) {
                MR::getJMapInfoArg0NoInit(rIter, pDest);
            }
        }

        kmCall(0x802BDE80, initKeySwitchSafeGetShadowDropLength); // overwrite call to getJMapInfoArg0NoInit


        /*****************************************************************************************************************/
        /* Fix crash wehn using Pull Star while standing on a HipDropSwitch.                                             */
        /*****************************************************************************************************************/
        f32 fixHipDropSwitchPullStarCrash(TVec3f& rVecA, TVec3f& rVecB) {
            return MR::isOnGroundPlayer() ? rVecA.dot(rVecB) : 0.0f;
        }

        kmCall(0x802AF938, fixHipDropSwitchPullStarCrash);
    }
}
