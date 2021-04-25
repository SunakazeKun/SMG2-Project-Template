#pragma once

#import "JKernel/JKRHeap.h"

namespace MR {
	class FunctorBase {
	public:
		virtual void operator()() = 0;
		virtual FunctorBase* clone(JKRHeap*) const = 0;
	};

	template<typename P, typename M>
	class FunctorV0M : public FunctorBase {
	public:
		inline FunctorV0M() {}

		virtual void operator()() {
			(mObjPtr->*mFuncPtr)();
		}

		virtual FunctorV0M<P, M>* clone(JKRHeap* heap) const {
			FunctorV0M<P, M>* functor = new(heap, 0) FunctorV0M<P, M>();
			functor->mObjPtr = mObjPtr;
			functor->mFuncPtr = mFuncPtr;
			return functor;
		}

		P mObjPtr;
		M mFuncPtr;
	};
};
