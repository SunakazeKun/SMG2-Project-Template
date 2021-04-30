#include "spack/Actor/SwingRope.h"
#include "mtx.h"

SwingRope::SwingRope(const char* pName) : LiveActor(pName) {
	_8C = 0.0f;
	_90 = 0.0f;
	_94 = 0.0f;
	_98 = 0.0f;

	PSMTXIdentity(_B4);

	_E4 = 0.0f;
	_E8 = 0.0f;
	_EC = 0.0f;
	_F8 = 0.0f;
	_FC = 0.0f;
	_100 = 0.0f;
	_104 = 0.0f;
	_108 = 0.0f;
	_10C = 0.0f;
	_110 = 0.0f;
	_114 = 0.0f;
	_118 = 0.0f;
	_11C = 0.0f;
}
