#include "Dolphin/os.h"
static void* __OSArenaHi = (void*)0x00000000;
static void* __OSArenaLo = (void*)0xffffffff;

/**
 * @note Address: 0x800EC424
 * @note Size: 0x8
 */
void* OSGetArenaHi(void) { return __OSArenaHi; }
/*
.loc_0x0:
  lwz       r3, -0x7098(r13)
  blr
*/

/**
 * @note Address: 0x800EC42C
 * @note Size: 0x8
 */
void* OSGetArenaLo(void) { return __OSArenaLo; }
/*
.loc_0x0:
  lwz       r3, -0x7CA0(r13)
  blr
*/

/**
 * @note Address: 0x800EC434
 * @note Size: 0x8
 */
void OSSetArenaHi(void* arena_hi)

{
	__OSArenaHi = arena_hi;
	return;
}
/*
.loc_0x0:
  stw       r3, -0x7098(r13)
  blr
*/

/**
 * @note Address: 0x800EC43C
 * @note Size: 0x8
 */
void OSSetArenaLo(void* arena_lo)

{
	__OSArenaLo = arena_lo;
	return;
}

/**
 * @note Address: N/A
 * @note Size: 0x2C
 */
#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))
void* OSAllocFromArenaLo(size_t size, u32 align)
{
  void* ptr;
  u8* arenaLo;

  ptr = OSGetArenaLo();
  arenaLo = ptr = (void*)ROUND(ptr, align);
  arenaLo += size;
  arenaLo = (u8*)ROUND(arenaLo, align);
  OSSetArenaLo(arenaLo);
  return ptr;
}

/**
 * @note Address: N/A
 * @note Size: 0x20
 */
void OSAllocFromArenaHi(void)
{
	// UNUSED FUNCTION
}
