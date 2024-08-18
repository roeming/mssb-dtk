#include "Dolphin/os.h"

void OSInitSemaphore(OSSemaphore *a, BOOL count)
{
    BOOL interrupts = OSDisableInterrupts();
    
    OSInitThreadQueue(&a->queue);
    
    a->num = count;

    OSRestoreInterrupts(interrupts);
}

s32 OSWaitSemaphore(OSSemaphore volatile *a)
{
    BOOL interrupts;
    s32 ret;

    interrupts = OSDisableInterrupts();
    {

        while ((ret = a->num) <= 0)
        {
            OSSleepThread((OSThreadQueue*)&a->queue);
        }
        
        a->num--;
    }
    OSRestoreInterrupts(interrupts);

    return ret;
}

s32 OSSignalSemaphore(OSSemaphore *a)
{
    BOOL interrupts;
    s32 ret;
    
    interrupts = OSDisableInterrupts();
    {
        ret = a->num;
        
        a->num++;

        OSWakeupThread(&a->queue);
    }
    OSRestoreInterrupts(interrupts);

    return ret;
}