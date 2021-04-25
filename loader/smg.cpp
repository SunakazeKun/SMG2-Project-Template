#include "kamekLoader.h"

void loadSMGBinary();

typedef void *(*JKRHeap_Alloc_t) (u32 size, s32 align, void *heap);
typedef void (*JKRHeap_Free_t) (void *buffer, void *heap);

struct loaderFunctionsEx {
	loaderFunctions base;
	JKRHeap_Alloc_t eggAlloc;
	JKRHeap_Free_t eggFree;
	void **gameHeapPtr;
	void **archiveHeapPtr;
};

void *allocAdapter(u32 size, bool isForCode, const loaderFunctions *funcs) {
	const loaderFunctionsEx *funcsEx = (const loaderFunctionsEx *)funcs;
    
	void **heapPtr = isForCode ? funcsEx->gameHeapPtr : funcsEx->archiveHeapPtr;
	return funcsEx->eggAlloc(size, 0x20, *heapPtr);
}
void freeAdapter(void *buffer, bool isForCode, const loaderFunctions *funcs) {
	const loaderFunctionsEx *funcsEx = (const loaderFunctionsEx *)funcs;
	void **heapPtr = isForCode ? funcsEx->gameHeapPtr : funcsEx->archiveHeapPtr;
	funcsEx->eggFree(buffer, *heapPtr);
}

#ifdef USA
const loaderFunctionsEx functions = {
	{(OSReport_t) 0x805B6210,
	(OSFatal_t) 0x805B8500,
	(DVDConvertPathToEntrynum_t) 0x805D1370,
	(DVDFastOpen_t) 0x805D1680,
	(DVDReadPrio_t) 0x805D1A50,
	(DVDClose_t) 0x805D1810,
	(sprintf_t) 0x80633CBC,
	allocAdapter,
	freeAdapter},
	(JKRHeap_Alloc_t) 0x80501310,
	(JKRHeap_Free_t) 0x80501360,
	(void **) 0x807D6110, // JKRHeap::sSystemHeap
	(void **) 0x807D6110 // JKRHeap::sRootHeap
};
#endif

#ifdef PAL
const loaderFunctionsEx functions = {
	{(OSReport_t) 0x805B6210,
	(OSFatal_t) 0x805B8500,
	(DVDConvertPathToEntrynum_t) 0x805D1370,
	(DVDFastOpen_t) 0x805D1680,
	(DVDReadPrio_t) 0x805D1A50,
	(DVDClose_t) 0x805D1810,
	(sprintf_t) 0x80633CBC,
	allocAdapter,
	freeAdapter},
	(JKRHeap_Alloc_t) 0x80501310,
	(JKRHeap_Free_t) 0x80501360,
	(void **) 0x807DB810, // JKRHeap::sSystemHeap
	(void **) 0x807DB810 // JKRHeap::sRootHeap
};
#endif

void unknownVersion()
{
	// can't do much here!
	// we can't output a message on screen without a valid OSFatal addr
	for (;;);
}

void loadSMGBinary()
{
	loadKamekBinaryFromDisc(&functions.base, "/CustomCode/CustomCode.bin");
}

// GameSystemException::init
kmBranch(0x804B7D38, loadSMGBinary);