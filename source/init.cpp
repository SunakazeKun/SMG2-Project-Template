#include "kamek.h"

typedef void (*Func)(void);

extern Func __ctor_loc;
extern Func __ctor_end;

void doCtors() {
    for (Func* f = &__ctor_loc; f < &__ctor_end; f++)
        (*f)();
}

kmBranch(0x804B66E8, doCtors);
