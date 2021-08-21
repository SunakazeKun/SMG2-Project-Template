#include "syati.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

/*
* Authors: shibbo
*/
void doCtors() {
    for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
        (*f)();
}

#if defined(TWN) || defined(KOR)
    kmBranch(0x804B6758, doCtors);
#else
    kmBranch(0x804B66E8, doCtors);
#endif
