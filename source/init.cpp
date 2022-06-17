#include "syati.h"

/*********************************************************************************************************************/
/* Call static initializers                                                                                          */
/*********************************************************************************************************************/
typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

void init() {
    for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
        (*f)();
    }
}

#if defined(TWN) || defined(KOR)
    kmBranch(0x804B6A64, init);
#else
    kmBranch(0x804B69F4, init);
#endif
