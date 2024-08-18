#include "musyx/musyx_priv.h"

void* salMalloc(u32 len) { return salHooks.malloc(len); }

void* salMallocPhysical(u32 len) { return salHooks.mallocPhysical(len); }

void salFree(void* addr) { salHooks.free(addr); }