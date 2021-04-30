#pragma once

#include "kamek.h"
#include "Util/Functor.h"

namespace SPack {
	template<typename P, typename M>
	MR::FunctorV0M<P, M> createFunctor(P p, M m) {
		MR::FunctorV0M<P, M> func;
		func.mObjPtr = p;
		func.mFuncPtr = m;
		return func;
	}

	void clamp(s32, s32, s32*);
	void clamp(f32, f32, f32*);
};
