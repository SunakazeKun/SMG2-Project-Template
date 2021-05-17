#include "spack/MR2/SwingRope.h"
#include "mtx.h"

/*
* Created by Aurum
* 
* All of its code still remains in SMG2... except for its constructor. This can be easily restored,
* however. Other than that, nothing else is required to make this functional again.
*/
SwingRope::SwingRope(const char* pName) : LiveActor(pName) {
	_8C.setAll<f32>(0.0f);
	_98 = 0.0f;
	_9C = false;
	_A0 = 0;
	_A4 = 0;
	_A8 = 0;
	_AC = NULL;
	_B0 = NULL;
	PSMTXIdentity(_B4);
	_E4 = 0.0f;
	_E8 = 0.0f;
	_EC = 0.0f;
	_F0 = NULL;
	_F4 = NULL;
	_F8 = 0.0f;
	_FC = 0.0f;
	_100 = 0.0f;
	_104.setAll<f32>(0.0f);
	_110.setAll<f32>(0.0f);
	_11C = 0.0f;
	_120 = 0;
}
