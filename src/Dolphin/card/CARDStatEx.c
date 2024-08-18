#include "Dolphin/card.h"
#include "mem.h"

s32 __CARDGetStatusEx(u32 chan, s32 cardFile, CARDDir* dst)
{
    CARDControl *ctrl;
    s32 ret;
    CARDDir* src;
    if (cardFile < 0 || cardFile >= CARD_MAX_FILE)
    {
        return CARD_RESULT_FATAL_ERROR;
    }

    ret = __CARDGetControlBlock(chan, &ctrl);
    if (ret < 0)
    {
        return ret;
    }
    
    src = &__CARDGetDirBlock(ctrl)->entries[cardFile];
    ret = __CARDIsReadable(ctrl, src);
    
    if (ret >= 0)
    {
        memcpy(dst, src, sizeof(*dst));
    }
    
    return __CARDPutControlBlock(ctrl, ret);
}

s32 __CARDSetStatusExAsync(u32 chan, s32 cardFile, CARDDir* dst, CARDCallback callback)
{
    u8 *maxFilePointer;
    CARDControl* ctrl;
    CARDDir* cardBlock;
    CARDDir* dirBlockEntry;
    u8 *fileName;
    s32 ret;

    if (cardFile < 0 || cardFile >= CARD_MAX_FILE || (dst->fileName[0] == 0xff)|| (dst->fileName[0] == '\0'))
    {
        return CARD_RESULT_FATAL_ERROR;
    }

    ret = __CARDGetControlBlock(chan, &ctrl);

    if (ret < 0)
    {
        return ret;
    }
    cardBlock = __CARDGetDirBlock(ctrl)->entries;
    dirBlockEntry = &cardBlock[cardFile];

    ret = __CARDIsWritable(ctrl, dirBlockEntry);
    if (ret < 0)
    {
        return __CARDPutControlBlock(ctrl, ret);
    }

    for (fileName = dst->fileName; fileName < &dst->fileName[CARD_FILENAME_MAX]; fileName++)
    {
        if (*fileName == 0) // as soon as the filename contains nothing, zero out the rest of the filename
        {
            while (++fileName < &dst->fileName[CARD_FILENAME_MAX])
            {
                *fileName = 0;
            }
            break;
        }
    }

    if (dst->permission & CARD_ATTR_GLOBAL)
    {
        memset(&dst->gameName, 0, sizeof(dst->gameName));
        memset(&dst->company, 0, sizeof(dst->company));
    }
    if (dst->permission & CARD_ATTR_COMPANY)
    {
        memset(&dst->gameName, 0, sizeof(dst->gameName));
    }

    if (memcmp(dirBlockEntry->fileName, dst->fileName, sizeof(dst->fileName)) ||
        memcmp(dirBlockEntry->gameName, dst->gameName, sizeof(dst->gameName)) ||
        memcmp(dirBlockEntry->company, dst->company, sizeof(dst->company)))
    {
        int i;
        for (i = 0; i < 0x7f; i++, cardBlock++)
        {
            if ((i != cardFile) &&
                (cardBlock->gameName[0] != 0xff) &&
                !memcmp(cardBlock->gameName, dst->gameName, sizeof(dst->gameName)) &&
                !memcmp(cardBlock->company, dst->company, sizeof(dst->company)) &&
                !memcmp(cardBlock->fileName, dst->fileName, sizeof(dst->fileName)))
            {
                return __CARDPutControlBlock(ctrl, CARD_RESULT_EXIST);
            }
        }
        memcpy(dirBlockEntry->fileName, dst->fileName, sizeof(dst->fileName));
        memcpy(dirBlockEntry->gameName, dst->gameName, sizeof(dst->gameName));
        memcpy(dirBlockEntry->company, dst->company, sizeof(dst->company));
    }

    dirBlockEntry->time = dst->time;
    dirBlockEntry->bannerFormat = dst->bannerFormat;
    dirBlockEntry->iconAddr = dst->iconAddr;
    dirBlockEntry->iconFormat = dst->iconFormat;
    dirBlockEntry->iconSpeed = dst->iconSpeed;
    dirBlockEntry->commentAddr = dst->commentAddr;
    dirBlockEntry->permission = dst->permission;
    dirBlockEntry->copyTimes = dst->copyTimes;
    ret = __CARDUpdateDir(chan, callback);
    if (ret < 0)
    {
        __CARDPutControlBlock(ctrl, ret);
    }

    return ret;
}