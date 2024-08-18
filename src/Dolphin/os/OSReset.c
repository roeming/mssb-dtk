#include "Dolphin/os.h"
#include "Dolphin/hw_regs.h"
#include "Dolphin/pad.h"

static OSResetQueue ResetFunctionQueue;
static u32 bootThisDol;

/**
 * @note Address: 0x800F02A4
 * @note Size: 0x84
 */
void OSRegisterResetFunction(OSResetFunctionInfo* info)
{
	OSResetFunctionInfo* tmp;
	OSResetFunctionInfo* iter;

	for (iter = ResetFunctionQueue.head; iter && iter->priority <= info->priority; iter = iter->next) {
		;
	}

	if (iter == NULL) {
		tmp = ResetFunctionQueue.tail;
		if (tmp == NULL) {
			ResetFunctionQueue.head = info;
		} else {
			tmp->next = info;
		}
		info->prev              = tmp;
		info->next              = NULL;
		ResetFunctionQueue.tail = info;
		return;
	}

	info->next = iter;
	tmp        = iter->prev;
	iter->prev = info;
	info->prev = tmp;
	if (tmp == NULL) {
		ResetFunctionQueue.head = info;
		return;
	}
	tmp->next = info;
}

/**
 * @note Address: N/A
 * @note Size: 0x94
 */
BOOL __OSCallResetFunctions(s32 priority) {
    OSResetFunctionInfo* iter;
    s32 retCode = 0;
	u32 _priority = 0;
	for (iter = ResetFunctionQueue.head; iter != NULL; _priority = iter->priority, iter = iter->next)
	{
		if (retCode && _priority != iter->priority)
		{
			break;
		} 
        retCode |= !iter->func(priority);
	}

	retCode |= !__OSSyncSram();
    return retCode ? FALSE : TRUE;
}

/**
 * @note Address: 0x800F0328
 * @note Size: 0x70
 */
ASM static void Reset(register s32 resetCode)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	b _jump1

_begin:
	mfspr r8, HID0
	ori r8, r8, 8
	mtspr HID0, r8
	isync
	sync
	nop
	b _preloop

_jump1:
	b _jump2

_preloop:
	mftb r5, 268
_loop:
	mftb r6, 268
	subf r7, r5, r6
	cmplwi r7, 0x1124
	blt _loop
	nop
	b _setPIReg

_jump2:
	b _jump3

_setPIReg:
	lis r8, 0xCC003000@h
	ori r8, r8, 0xCC003000@l
	li r4, 3
	stw r4, 0x24(r8)
	stw r3, 0x24(r8)
	nop
	b _noptrap

_jump3:
	b _jump4

_noptrap:
	nop
	b _noptrap

_jump4:
	b _begin
#endif // clang-format on
}

/**
 * @note Address: N/A
 * @note Size: 0x68
 */
static void KillThreads()
{
	OSThread* thread;
	OSThread* next;

	for (thread = __OSActiveThreadQueue.head; thread; thread = next) {
		next = thread->linkActive.next;
		switch (thread->state) {
		case 1:
		case 4:
			OSCancelThread(thread);
			break;
		default:
			break;
		}
	}
}

/**
 * @note Address: 0x800F0398
 * @note Size: 0x48
 */
void __OSDoHotReset(s32 code)
{
	OSDisableInterrupts();
	__VIRegs[VI_DISP_CONFIG] = 0;
	ICFlashInvalidate();
	Reset(code * 8);
}

/**
 * @note Address: 0x800F03E0
 * @note Size: 0x2BC
 */
#pragma dont_inline on
void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu)
{
	BOOL rc;
	BOOL disableRecalibration;
	u32 stackManip;

	OSDisableScheduler();

	if (reset == OS_RESET_HOTRESET && forceMenu) {
		OSSram* sram;

		sram = __OSLockSram();
		sram->flags |= 0x40;
		__OSUnlockSram(TRUE);
		
		resetCode = 0;
	}
	if (reset == OS_RESET_SHUTDOWN || (reset == OS_RESET_RESTART && (bootThisDol || (resetCode + 0x3fff0000 == 0))))
	{
		__OSStopAudioSystem();
		disableRecalibration = __PADDisableRecalibration(1);
		while (!__OSCallResetFunctions(FALSE));
		while(!__OSSyncSram());
		{
			OSDisableInterrupts();
			__OSCallResetFunctions(1);

			LCDisable();
			__PADDisableRecalibration(disableRecalibration);			
			KillThreads();
		}
	}
	else {
		__OSStopAudioSystem();
		while(!__OSCallResetFunctions(FALSE));
		while(!__OSSyncSram());
		{
			OSDisableInterrupts();
			__OSCallResetFunctions(TRUE);
			LCDisable();
			KillThreads();
		}
	}
	if (reset == OS_RESET_HOTRESET)
	{
		OSDisableInterrupts();
		__VIRegs[1] = 0;
		ICFlashInvalidate();
		Reset(resetCode << 3);
	}
	else if (reset == OS_RESET_RESTART)
	{
		if (forceMenu == 1)
		{
			OSReport("OSResetSystem(): You can't specify TRUE to forceMenu if you restart. Ignored\n");
		}
		OSEnableScheduler();
		__OSReboot(resetCode, bootThisDol);
	}

	memset(OSPhysicalToCached(0x40), 0, 0xCC - 0x40);
	memset(OSPhysicalToCached(0xD4), 0, 0xE8 - 0xD4);
	memset(OSPhysicalToCached(0xF4), 0, 0xF8 - 0xF4);
	memset(OSPhysicalToCached(0x3000), 0, 0xC0);
	memset(OSPhysicalToCached(0x30C8), 0, 0xD4 - 0xC8);
	memset(OSPhysicalToCached(0x30E2), 0, 1);

	// __PADDisableRecalibration(disableRecalibration);
}

/**
 * @note Address: 0x800F069C
 * @note Size: 0x34
 */
u32 OSGetResetCode()
{
	if (__OSRebootParams._00 != 0) {
		return __OSRebootParams._04 | 0x80000000;
	}

	return ((__PIRegs[PI_RESETCODE] & ~7) >> 3);
}
