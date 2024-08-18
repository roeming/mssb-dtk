#include "Dolphin/card.h"
#include "Dolphin/dsp.h"
#include "Dolphin/dvd.h"
#include "Dolphin/os.h"

static void DeleteCallback(s32 chan, s32 result)
{
  CARDControl *card;
  CARDCallback callback;

  card = &__CARDBlock[chan];
  callback = card->apiCallback;
  card->apiCallback = 0;

  if (result < 0)
  {
    goto error;
  }

  result = __CARDFreeBlock(chan, card->startBlock, callback);
  if (result < 0)
  {
    goto error;
  }
  return;

error:
  __CARDPutControlBlock(card, result);
  if (callback)
  {
    callback(chan, result);
  }
}

s32 CARDDeleteAsync(s32 chan, const char *fileName, CARDCallback callback)
{
  CARDControl *card;
  s32 fileNo;
  s32 result;
  CARDDirectoryBlock *dir;
  CARDDir *ent;

  result = __CARDGetControlBlock(chan, &card);
  if (result < 0)
  {
    return result;
  }
  result = __CARDGetFileNo(card, fileName, &fileNo);
  if (result < 0)
  {
    return __CARDPutControlBlock(card, result);
  }
  if (__CARDIsOpened(card, fileNo))
  {
    return __CARDPutControlBlock(card, CARD_RESULT_BUSY);
  }

  dir = __CARDGetDirBlock(card);
  ent = &dir->entries[fileNo];
  card->startBlock = ent->startBlock;
  memset(ent, 0xff, sizeof(CARDDir));

  card->apiCallback = callback ? callback : __CARDDefaultApiCallback;
  result = __CARDUpdateDir(chan, DeleteCallback);
  if (result < 0)
  {
    __CARDPutControlBlock(card, result);
  }
  return result;
}

s32 CARDFastDelete(s32 chan, char *fileName)
{
  s32 res = CARDDeleteAsync(chan, fileName, __CARDSyncCallback);

  return res < 0 ? res : __CARDSync(chan);
}