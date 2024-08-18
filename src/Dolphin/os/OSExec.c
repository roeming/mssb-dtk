#include "Dolphin/os.h"
#include "stl/stdio.h"
#include "string.h"
#include "Dolphin/hw_regs.h"

static int lbl_803CBD98;
static int CachedApploaderAddr;

typedef struct
{
    u32 _00[0x800];
} ArgsStruct;

#define ARG_SIZE 0x2000
BOOL PackArgs(void *arglist, int argc, char *argv[])
{
    long numArgs;
    char *pArgList;
    char *ptr;
    char **list;
    u32 i;

    pArgList = (char *)arglist;
    memset(pArgList, 0, ARG_SIZE);

    if (!argc)
    {
        ((u32 *)pArgList)[2] = 0;
    }
    else
    {
        numArgs = argc;
        ptr = pArgList + ARG_SIZE;

        while (--argc >= 0)
        {
            ptr -= strlen(argv[argc]) + 1;
            strcpy(ptr, argv[argc]);
            argv[argc] = (char *)(ptr - pArgList);
        }

        ptr = (char *)pArgList + ROUND_DOWN(ptr - pArgList, 4);
        ptr -= (numArgs + 1) * 4;
        list = (char **)ptr;

        for (i = 0; i < numArgs + 1; i++)
            list[i] = argv[i];

        ptr -= sizeof(int);
        *(int *)ptr = numArgs;

        ((u32 *)pArgList)[2] = ptr - pArgList;
    }

    return TRUE;
}

typedef void (*runFunc)(void);
#ifdef __MWERKS__ // clang-format off
ASM void Run(register runFunc v)
{
    fralloc    
    bl ICFlashInvalidate
    sync
    isync
    mtlr v
    blr
}
#endif // clang-format on

static void ReadDisc(void *addr, s32 len, s32 offset)
{
    DVDCommandBlock bloc;

    DVDReadAbsAsyncPrio(&bloc, addr, len, offset, 0, 0);

    while (DVDGetCommandBlockStatus(&bloc))
    {
        if (!DVDCheckDisk())
        {
            __OSDoHotReset(0);
        }
    }
}

void Callback(void)
{
    lbl_803CBD98 = 1;
}
extern u32 OS_RESET_CODE AT_ADDRESS(0x800030F0);
extern int OS_APPLOADER_ADDR AT_ADDRESS(0x800030F4);

typedef struct
{
    int _00;
    int _04;
    int _08;
    int _0c;
    int _10;
    int _14;
} ApploaderStruct;

#define APPLOADER_ADDR ((void *)0x81200000)
extern int IPL_ADDR AT_ADDRESS(0x81300000);

void __OSGetExecParams(void *outParams)
{
    if (0x80000000 <= OS_RESET_CODE)
    {
        memcpy(outParams, (void *)OS_RESET_CODE, 0x1c);
    }
    else
    {
        *(u32 *)outParams = 0;
    }
}

u32 GetApploaderPosition(void)
{
    u32 stackManip;
    if (CachedApploaderAddr)
    {
        return CachedApploaderAddr;
    }

    if (OS_APPLOADER_ADDR)
    {
        DVDCommandBlock bloc;
        void *ptr = (void *)OSAllocFromArenaLo(0x40, 0x20);
        DVDReadAbsAsyncPrio(&bloc, ptr, 0x40, OS_APPLOADER_ADDR, 0, 0);

        while (DVDGetCommandBlockStatus(&bloc))
        {
            if (!DVDCheckDisk())
            {
                __OSDoHotReset(0);
            }
        }

        CachedApploaderAddr = OS_APPLOADER_ADDR + ((u32 *)ptr)[0xe];
    }
    else
    {
        CachedApploaderAddr = 0x2440;
    }

    return CachedApploaderAddr;
}

#pragma dont_inline on
typedef int (*unk3ParamFunc)(void *a, void *b, void *c);
typedef u32 (*unk2ParamFunc)(void *a, void *b);
typedef u32 (*unk1ParamFunc)(void *a);
typedef u32 (*unk0ParamFunc)();
typedef struct
{
    u32 _00;
    u32 _04;
    u32 _08;
    u32 _0C;
    u32 _10;
    u32 _14;
    void *_18;
} _1C_Struct;
static void __OSBootDolSimple(u32 v, u32 b, void *start, void *end, int argc, u32 count, ArgsStruct *argv)
{
    _1C_Struct *src;
    DVDCommandBlock bloc;
    _1C_Struct *src2;
    ApploaderStruct *allocated;
    struct {
        u32 _00;
        u32 _04;
        u32 _08;
        u32 _0C;
        u32 _10;
        u32 _14;
        u32 _1C;
    }_c_LOOKATME;
    int ret;

    int interrupts;

    interrupts = OSDisableInterrupts();
    src = (_1C_Struct *)OSAllocFromArenaLo(sizeof(_1C_Struct), 1);
    src->_00 = 1;
    src->_04 = b;
    src->_0C = (u32)start;
    src->_10 = (u32)end;
    src->_14 = argc;
    if (!argc)
    {
        src->_18 = OSAllocFromArenaLo(ARG_SIZE, 1);
        PackArgs(src->_18, count, (char **)argv);
    }
    DVDInit();
    DVDSetAutoInvalidation(1);
    DVDResume();
    lbl_803CBD98 = 0;
    __DVDPrepareResetAsync(Callback);
    __OSMaskInterrupts(0xffffffe0);
    __OSUnmaskInterrupts(0x400);
    OSEnableInterrupts();
    while (lbl_803CBD98 != 1)
    {
        if (!DVDCheckDisk())
        {
            __OSDoHotReset(0);
        }
    }
    if ((__OSIsGcam == 0))
    {
        if (DVDGetCurrentDiskID()->streaming ? 1 : 0)
        {
            AISetStreamVolLeft(0);
            AISetStreamVolRight(0);
            DVDCancelStreamAsync(&bloc, 0);
            while (DVDGetCommandBlockStatus(&bloc))
            {
                if (!DVDCheckDisk())
                {
                    __OSDoHotReset(0);
                }
            }
            AISetStreamPlayState(0);
        }
    }

    src2 = (_1C_Struct *)OSAllocFromArenaLo(0x20, 0x20);
    ReadDisc(src2, 0x20, GetApploaderPosition());

    ReadDisc(APPLOADER_ADDR, ROUND_UP(src2->_14, 32), GetApploaderPosition() + 0x20);
    ICInvalidateRange(APPLOADER_ADDR, ROUND_UP(src2->_14, 32));

    if (strncmp((char *)src2, "2004/02/01", 10) > 0 ? 1 : 0)
    {
        if (v + 0x10000 == 0xffff)
        {
            DVDCommandBlock block2;
            DVDCommandBlock bloc;
            int *alloc;
            int thisCachedApploaderAddr = CachedApploaderAddr;
            switch (thisCachedApploaderAddr)
            {
            case 0:
                if (OS_APPLOADER_ADDR)
                {
                    argv = (void *)OSAllocFromArenaLo(0x40, 0x20);
                    DVDReadAbsAsyncPrio(&bloc, argv, 0x40, OS_APPLOADER_ADDR, 0, 0);

                    while (DVDGetCommandBlockStatus(&bloc))
                    {
                        if (!DVDCheckDisk())
                        {
                            __OSDoHotReset(0);
                        }
                    }

                    CachedApploaderAddr = OS_APPLOADER_ADDR + ((u32 *)argv)[0xe];
                }
                else
                {
                    CachedApploaderAddr = 0x2440;
                }
                thisCachedApploaderAddr = CachedApploaderAddr;
            }
            argv = (ArgsStruct *)(thisCachedApploaderAddr + argv->_00[5]);
            argv = (ArgsStruct *)(((u32 *)argv) + 8);
        }
        src->_08 = v;
        ((unk3ParamFunc)argv->_00[4])(&_c_LOOKATME._04, &_c_LOOKATME._04, &_c_LOOKATME._04);

        src2 = OSAllocFromArenaLo(sizeof(_1C_Struct), 1);
        memcpy(src2, argv, sizeof(_1C_Struct));

        OS_RESET_CODE = (u32)src2;

        ((unk1ParamFunc)_c_LOOKATME._04)(OSReport);

        OSSetArenaLo(src2);
        while (((unk3ParamFunc)_c_LOOKATME._00)(&_c_LOOKATME._04, &_c_LOOKATME._08, &_c_LOOKATME._0C))
        {
            DVDCommandBlock v;
            DVDReadAbsAsyncPrio(&v, (void *)_c_LOOKATME._04, (u32)_c_LOOKATME._08, (u32)_c_LOOKATME._0C, 0, 0);
            while (DVDGetCommandBlockStatus(&v))
            {
                if (!DVDCheckDisk())
                {
                    __OSDoHotReset(0);
                }
            }
        }

        {
            _1C_Struct *s ;
            ret = ((unk0ParamFunc)_c_LOOKATME._08)();

            s = OSAllocFromArenaLo(sizeof(_1C_Struct), 1);
            memcpy(s, src, sizeof(_1C_Struct));
            OS_RESET_CODE = (u32)s;
            __PIRegs[9] = 7;
            OSDisableInterrupts();
            Run((runFunc)ret);
            return;
        }
    }

    BOOT_REGION_START = (u32)start;
    BOOT_REGION_END = (u32)end;
    BOOT_REGION_UNK = 1;
    {
        int newAppPos = GetApploaderPosition() + 0x20 + argv->_00[5];
        DVDReadAbsAsyncPrio(&bloc, (void *)OS_BOOTROM_ADDR, ROUND_UP(argv->_00[6], 32), newAppPos, 0, 0);
    }
    while (DVDGetCommandBlockStatus(&bloc))
    {
        if (!DVDCheckDisk())
        {
            __OSDoHotReset(0);
        }
    }

    ICInvalidateRange((void *)OS_BOOTROM_ADDR, ROUND_UP(argv->_00[6], 32));
    OSDisableInterrupts();
    ICFlashInvalidate();
    Run((runFunc)OS_BOOTROM_ADDR);
}

void __OSBootDol(u32 a, u32 b, u32 *data)
{
    char buff[0x10];
    ArgsStruct *ppcVar3;
    int count, i;
    void *start, *end;

    OSGetSaveRegion(&start, &end);

    sprintf(buff, "%d", a);

    count = 0;
    if (data)
    {
        while (data[count])
        {
            count++;
        }
    }

    ppcVar3 = (ArgsStruct *)OSAllocFromArenaLo((count + 2) * sizeof(u32), 1);
    ppcVar3->_00[0] = (u32)buff;

    count++;
    for (i = 1; i < count; i++)
    {
        ppcVar3->_00[i] = data[i - 1];
    }

    __OSBootDolSimple(0xffffffff, b, start, end, 0, count, ppcVar3);
}
#pragma dont_inline reset