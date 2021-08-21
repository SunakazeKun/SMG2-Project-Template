#pragma once

#include "Util/Functor.h"

namespace SPack {
	template<typename P, typename M>
	MR::FunctorV0M<P, M> createFunctor(P pObject, M pFunc) {
		MR::FunctorV0M<P, M> func;
		func.mObjPtr = pObject;
		func.mFuncPtr = pFunc;
		return func;
	}
};
