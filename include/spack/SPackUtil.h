#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "Util/Functor.h"

namespace SPack {
	/*
	* Functors
	*/
	template<typename P, typename M>
	MR::FunctorV0M<P, M> createFunctor(P p, M m) {
		MR::FunctorV0M<P, M> func;
		func.mObjPtr = p;
		func.mFuncPtr = m;
		return func;
	}

	/*
	* Math helpers
	*/
	void clamp(s32, s32, s32*);
	void clamp(f32, f32, f32*);
	
	const f32 RAD2DEG = 57.295776f;

	/*
	* Actor helpers
	*/
	void initPosRotFromRailRider(LiveActor*);
	void updatePosRotFromRailRider(LiveActor*);
};
