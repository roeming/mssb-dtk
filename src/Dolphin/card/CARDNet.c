#include "Dolphin/card.h"

u16 __CARDVendorID = 0xFFFF;
u8 __CARDPermMask  = 28;

/**
 * @note Address: 0x800DAB00
 * @note Size: 0xC4
 */
s32 CARDGetSerialNo(s32 channel, u64* serialNo)
{
	CARDControl* card;
	CARDID* id;
	int i;
	u64 code;
	s32 result;

	if (!(0 <= channel && channel < 2)) {
		return CARD_RESULT_FATAL_ERROR;
	}

	result = __CARDGetControlBlock(channel, &card);
	if (result < 0) {
		return result;
	}

	id = &card->workArea->header.id;
	for (code = 0, i = 0; i < sizeof(id->serial) / sizeof(u64); ++i) {
		code ^= *(u64*)&id->serial[sizeof(u64) * i];
	}
	*serialNo = code;

	return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDSetAttributesAsync(s32 channel, s32 cardFile, u8 permissions, CARDCallback callback)
{
	CARDDir dir;
	s32 res;
	if (permissions & ~__CARDPermMask)
	{
		return CARD_RESULT_NOPERM;
	}

	res = __CARDGetStatusEx(channel, cardFile, &dir);
	if (res < 0)
	{
		return res;
	}

	if (
		((dir.permission & 0x20u) && !(permissions & 0x20u)) ||
		((dir.permission & 0x40u) && !(permissions & 0x40u))
	   )
	{
		return CARD_RESULT_NOPERM;
	}

	if (((permissions & 0x20u) && (permissions & 0x40u)) ||
		((permissions & 0x20u) && (dir.permission & 0x40u)) ||
		((permissions & 0x40u) && (dir.permission & 0x20u)))
	{
		return CARD_RESULT_NOPERM;
	}
	
	dir.permission = permissions;
	return __CARDSetStatusExAsync(channel, cardFile, &dir, callback);
}

s32 CARDSetAttributes(s32 channel, s32 cardFile, u8 permissions)
{
	s32 ret = CARDSetAttributesAsync(channel, cardFile, permissions, __CARDSyncCallback);
	if(ret < 0)
	{
		return ret;
	}
	return __CARDSync(channel);
}