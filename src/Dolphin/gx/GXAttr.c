#include "Dolphin/gx.h"
#include "Dolphin/GX/GXHardware.h"

/**
 * @note Address: N/A
 * @note Size: 0x8C
 */
static void __GXXfVtxSpecs(void)
{
	u32 normCount, colorCount, texCount;

	normCount = gx->hasBiNrms ? 2 : (gx->hasNrms ? 1 : 0);

	// Both fields in one access
	colorCount = 33 - __cntlzw((gx->vcdLo & (0xf << 0xd)) >> 0xd);
	colorCount /= 2; // equivalent to /=2 and >>= 1

	// All 16 assigned bits in VCD_Hi
	texCount = 33 - __cntlzw((gx->vcdHi & (0xffff << 0)) >> 0);
	texCount /= 2; // equivalent to /=2 and >>= 1

	GX_XF_LOAD_REG(GX_XF_REG_INVERTEXSPEC, (colorCount) | (normCount << 2) | (texCount << 4));
	gx->bpSentNot = GX_TRUE;

	return;
}

/**
 * @note Address: 0x800E4284
 * @note Size: 0x26C
 */

void GXSetVtxDesc(GXAttr attr, GXAttrType type)
{
	switch (attr) {
	case GX_VA_PNMTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_POSMTXIDX_ST, GX_CP_VCD_LO_POSMTXIDX_END);
		break;
	case GX_VA_TEX0MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX0MTXIDX_ST, GX_CP_VCD_LO_TEX0MTXIDX_END);
		break;
	case GX_VA_TEX1MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX1MTXIDX_ST, GX_CP_VCD_LO_TEX1MTXIDX_END);
		break;
	case GX_VA_TEX2MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX2MTXIDX_ST, GX_CP_VCD_LO_TEX2MTXIDX_END);
		break;
	case GX_VA_TEX3MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX3MTXIDX_ST, GX_CP_VCD_LO_TEX3MTXIDX_END);
		break;
	case GX_VA_TEX4MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX4MTXIDX_ST, GX_CP_VCD_LO_TEX4MTXIDX_END);
		break;
	case GX_VA_TEX5MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX5MTXIDX_ST, GX_CP_VCD_LO_TEX5MTXIDX_END);
		break;
	case GX_VA_TEX6MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX6MTXIDX_ST, GX_CP_VCD_LO_TEX6MTXIDX_END);
		break;
	case GX_VA_TEX7MTXIDX:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX7MTXIDX_ST, GX_CP_VCD_LO_TEX7MTXIDX_END);
		break;
	case GX_VA_POS:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_POS_ST, GX_CP_VCD_LO_POS_END);
		break;
	case GX_VA_NRM:
		if (type != GX_NONE) {
			gx->hasNrms   = TRUE;
			gx->hasBiNrms = FALSE;
			gx->nrmType   = type;
		} else {
			gx->hasNrms = FALSE;
		}
		break;
	case GX_VA_NBT:
		if (type != GX_NONE) {
			gx->hasBiNrms = TRUE;
			gx->hasNrms   = FALSE;
			gx->nrmType   = type;
		} else {
			gx->hasBiNrms = FALSE;
		}
		break;
	case GX_VA_CLR0:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_CLRDIF_ST, GX_CP_VCD_LO_CLRDIF_END);
		break;
	case GX_VA_CLR1:
		GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_CLRSPEC_ST, GX_CP_VCD_LO_CLRSPEC_END);
		break;
	case GX_VA_TEX0:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX0COORD_ST, GX_CP_VCD_HI_TEX0COORD_END);
		break;
	case GX_VA_TEX1:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX1COORD_ST, GX_CP_VCD_HI_TEX1COORD_END);
		break;
	case GX_VA_TEX2:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX2COORD_ST, GX_CP_VCD_HI_TEX2COORD_END);
		break;
	case GX_VA_TEX3:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX3COORD_ST, GX_CP_VCD_HI_TEX3COORD_END);
		break;
	case GX_VA_TEX4:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX4COORD_ST, GX_CP_VCD_HI_TEX4COORD_END);
		break;
	case GX_VA_TEX5:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX5COORD_ST, GX_CP_VCD_HI_TEX5COORD_END);
		break;
	case GX_VA_TEX6:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX6COORD_ST, GX_CP_VCD_HI_TEX6COORD_END);
		break;
	case GX_VA_TEX7:
		GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX7COORD_ST, GX_CP_VCD_HI_TEX7COORD_END);
		break;
	}

	if (gx->hasNrms || gx->hasBiNrms) {
		GX_SET_REG(gx->vcdLo, gx->nrmType, GX_CP_VCD_LO_NRM_ST, GX_CP_VCD_LO_NRM_END);
	} else {
		GX_SET_REG(gx->vcdLo, GX_NONE, GX_CP_VCD_LO_NRM_ST, GX_CP_VCD_LO_NRM_END);
	}

	gx->dirtyState |= GX_DIRTY_VCD;
}

/**
 * @note Address: N/A
 * @note Size: 0x288
 */
void GXSetVtxDescv(GXVtxDescList* attrPtr)
{
	GXAttrType type;
	GXAttr attr;

	for (attrPtr; attrPtr->mAttr != GX_VA_NULL; attrPtr++)
	{
		type = attrPtr->mType;
		attr = attrPtr->mAttr;
		switch (attr)
		{
		case GX_VA_PNMTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_POSMTXIDX_ST, GX_CP_VCD_LO_POSMTXIDX_END);
			break;
		case GX_VA_TEX0MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX0MTXIDX_ST, GX_CP_VCD_LO_TEX0MTXIDX_END);
			break;
		case GX_VA_TEX1MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX1MTXIDX_ST, GX_CP_VCD_LO_TEX1MTXIDX_END);
			break;
		case GX_VA_TEX2MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX2MTXIDX_ST, GX_CP_VCD_LO_TEX2MTXIDX_END);
			break;
		case GX_VA_TEX3MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX3MTXIDX_ST, GX_CP_VCD_LO_TEX3MTXIDX_END);
			break;
		case GX_VA_TEX4MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX4MTXIDX_ST, GX_CP_VCD_LO_TEX4MTXIDX_END);
			break;
		case GX_VA_TEX5MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX5MTXIDX_ST, GX_CP_VCD_LO_TEX5MTXIDX_END);
			break;
		case GX_VA_TEX6MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX6MTXIDX_ST, GX_CP_VCD_LO_TEX6MTXIDX_END);
			break;
		case GX_VA_TEX7MTXIDX:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_TEX7MTXIDX_ST, GX_CP_VCD_LO_TEX7MTXIDX_END);
			break;
		case GX_VA_POS:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_POS_ST, GX_CP_VCD_LO_POS_END);
			break;
		case GX_VA_NRM:
			if (type != GX_NONE)
			{
				gx->hasNrms = TRUE;
				gx->hasBiNrms = FALSE;
				gx->nrmType = type;
			}
			else
			{
				gx->hasNrms = FALSE;
			}
			break;
		case GX_VA_NBT:
			if (type != GX_NONE)
			{
				gx->hasBiNrms = TRUE;
				gx->hasNrms = FALSE;
				gx->nrmType = type;
			}
			else
			{
				gx->hasBiNrms = FALSE;
			}
			break;
		case GX_VA_CLR0:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_CLRDIF_ST, GX_CP_VCD_LO_CLRDIF_END);
			break;
		case GX_VA_CLR1:
			GX_SET_REG(gx->vcdLo, type, GX_CP_VCD_LO_CLRSPEC_ST, GX_CP_VCD_LO_CLRSPEC_END);
			break;
		case GX_VA_TEX0:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX0COORD_ST, GX_CP_VCD_HI_TEX0COORD_END);
			break;
		case GX_VA_TEX1:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX1COORD_ST, GX_CP_VCD_HI_TEX1COORD_END);
			break;
		case GX_VA_TEX2:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX2COORD_ST, GX_CP_VCD_HI_TEX2COORD_END);
			break;
		case GX_VA_TEX3:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX3COORD_ST, GX_CP_VCD_HI_TEX3COORD_END);
			break;
		case GX_VA_TEX4:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX4COORD_ST, GX_CP_VCD_HI_TEX4COORD_END);
			break;
		case GX_VA_TEX5:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX5COORD_ST, GX_CP_VCD_HI_TEX5COORD_END);
			break;
		case GX_VA_TEX6:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX6COORD_ST, GX_CP_VCD_HI_TEX6COORD_END);
			break;
		case GX_VA_TEX7:
			GX_SET_REG(gx->vcdHi, type, GX_CP_VCD_HI_TEX7COORD_ST, GX_CP_VCD_HI_TEX7COORD_END);
			break;
		}
	}

	if (gx->hasNrms || gx->hasBiNrms)
	{
		GX_SET_REG(gx->vcdLo, gx->nrmType, GX_CP_VCD_LO_NRM_ST, GX_CP_VCD_LO_NRM_END);
	}
	else
	{
		GX_SET_REG(gx->vcdLo, GX_NONE, GX_CP_VCD_LO_NRM_ST, GX_CP_VCD_LO_NRM_END);
	}

	gx->dirtyState |= GX_DIRTY_VCD;
}

/**
 * @note Address: 0x800E44F0
 * @note Size: 0xBC
 */
void __GXSetVCD(void)
{

	GX_CP_LOAD_REG(GX_CP_REG_VCD_LO, gx->vcdLo);
	GX_CP_LOAD_REG(GX_CP_REG_VCD_HI, gx->vcdHi);

	__GXXfVtxSpecs();
}

/**
 * @note Address: 0x800E45AC
 * @note Size: 0x124
 */
void __GXCalculateVLim(void)
{
	static u8 tbl1[] = { 0, 4, 1, 2 };
	static u8 tbl2[] = { 0, 8, 1, 2 };
	static u8 tbl3[] = { 0, 12, 1, 2 };

	u32 vlim;
	u32 vcdLo;
	u32 vcdHi;
	s32 compCnt;

	if (gx->vNum == 0) {
		return;
	}

	vcdLo = gx->vcdLo;
	vcdHi = gx->vcdHi;

	// GXCompCnt bit of normal parameters
	compCnt = gx->vatA[GX_VTXFMT0];
	compCnt = (compCnt & 0x200) >> 9;

	vlim = GX_GET_REG(vcdLo, GX_CP_VCD_LO_POSMTXIDX_ST, GX_CP_VCD_LO_POSMTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX0MTXIDX_ST, GX_CP_VCD_LO_TEX0MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX1MTXIDX_ST, GX_CP_VCD_LO_TEX1MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX2MTXIDX_ST, GX_CP_VCD_LO_TEX2MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX3MTXIDX_ST, GX_CP_VCD_LO_TEX3MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX4MTXIDX_ST, GX_CP_VCD_LO_TEX4MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX5MTXIDX_ST, GX_CP_VCD_LO_TEX5MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX6MTXIDX_ST, GX_CP_VCD_LO_TEX6MTXIDX_END);
	vlim += GX_GET_REG(vcdLo, GX_CP_VCD_LO_TEX7MTXIDX_ST, GX_CP_VCD_LO_TEX7MTXIDX_END);

	vlim += tbl3[GX_GET_REG(vcdLo, GX_CP_VCD_LO_POS_ST, GX_CP_VCD_LO_POS_END)];
	vlim += tbl3[GX_GET_REG(vcdLo, GX_CP_VCD_LO_NRM_ST, GX_CP_VCD_LO_NRM_END)] * (compCnt == GX_NRM_NBT ? 3 : 1);
	vlim += tbl1[GX_GET_REG(vcdLo, GX_CP_VCD_LO_CLRDIF_ST, GX_CP_VCD_LO_CLRDIF_END)];
	vlim += tbl1[GX_GET_REG(vcdLo, GX_CP_VCD_LO_CLRSPEC_ST, GX_CP_VCD_LO_CLRSPEC_END)];

	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX0COORD_ST, GX_CP_VCD_HI_TEX0COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX1COORD_ST, GX_CP_VCD_HI_TEX1COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX2COORD_ST, GX_CP_VCD_HI_TEX2COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX3COORD_ST, GX_CP_VCD_HI_TEX3COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX4COORD_ST, GX_CP_VCD_HI_TEX4COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX5COORD_ST, GX_CP_VCD_HI_TEX5COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX6COORD_ST, GX_CP_VCD_HI_TEX6COORD_END)];
	vlim += tbl2[GX_GET_REG(vcdHi, GX_CP_VCD_HI_TEX7COORD_ST, GX_CP_VCD_HI_TEX7COORD_END)];

	gx->vLim = vlim;
}

/**
 * @note Address: N/A
 * @note Size: 0x1B4
 */
void GXGetVtxDesc(GXAttr attr, GXAttrType* type)
{
	  GXAttrType GVar1;
  
  switch(attr) {
  case GX_VA_PNMTXIDX:
	GVar1 = __GXData->vcdLo & 1;
    break;
  case GX_VA_TEX0MTXIDX:
    GVar1 = __GXData->vcdLo >> 1 & 1;
    break;
  case GX_VA_TEX1MTXIDX:
    GVar1 = __GXData->vcdLo >> 2 & 1;
    break;
  case GX_VA_TEX2MTXIDX:
    GVar1 = __GXData->vcdLo >> 3 & 1;
    break;
  case GX_VA_TEX3MTXIDX:
    GVar1 = __GXData->vcdLo >> 4 & 1;
    break;
  case GX_VA_TEX4MTXIDX:
    GVar1 = __GXData->vcdLo >> 5 & 1;
    break;
  case GX_VA_TEX5MTXIDX:
    GVar1 = __GXData->vcdLo >> 6 & 1;
    break;
  case GX_VA_TEX6MTXIDX:
    GVar1 = __GXData->vcdLo >> 7 & 1;
    break;
  case GX_VA_TEX7MTXIDX:
    GVar1 = __GXData->vcdLo >> 8 & 1;
    break;
  case GX_VA_POS:
    GVar1 = __GXData->vcdLo >> 9 & 3;
    break;
  case GX_VA_NRM:
    if (__GXData->hasNrms) {
      GVar1 = __GXData->vcdLo >> 0xb & 3;
    }
    else {
      GVar1 = GX_NONE;
    }
    break;
  case GX_VA_NBT:
    if (__GXData->hasBiNrms) {
      GVar1 = __GXData->vcdLo >> 0xb & 3;
    }
    else {
      GVar1 = GX_NONE;
    }
	break;
  case GX_VA_CLR0:
    GVar1 = __GXData->vcdLo >> 0xd & 3;
    break;
  case GX_VA_CLR1:
    GVar1 = __GXData->vcdLo >> 0xf & 3;
    break;
  case GX_VA_TEX0:
    GVar1 = __GXData->vcdHi & 3;
    break;
  case GX_VA_TEX1:
    GVar1 = __GXData->vcdHi >> 2 & 3;
    break;
  case GX_VA_TEX2:
    GVar1 = __GXData->vcdHi >> 4 & 3;
    break;
  case GX_VA_TEX3:
    GVar1 = __GXData->vcdHi >> 6 & 3;
    break;
  case GX_VA_TEX4:
    GVar1 = __GXData->vcdHi >> 8 & 3;
    break;
  case GX_VA_TEX5:
    GVar1 = __GXData->vcdHi >> 10 & 3;
    break;
  case GX_VA_TEX6:
    GVar1 = __GXData->vcdHi >> 0xc & 3;
    break;
  case GX_VA_TEX7:
    GVar1 = __GXData->vcdHi >> 0xe & 3;
    break;
  default:
    GVar1 = GX_NONE;
    break;
  }
  *type = GVar1;
}

/**
 * @note Address: N/A
 * @note Size: 0x90
 */
void GXGetVtxDescv(GXVtxDescList* list)
{
	GXAttr attr = GX_VA_PNMTXIDX;
	do {
		list[attr].mAttr = attr;
		GXGetVtxDesc(attr, &list[attr].mType);
		attr++;
	} while (attr <= GX_VA_TEX7);
	
	list[attr].mAttr = GX_VA_NBT;
	GXGetVtxDesc(GX_VA_NBT, &list[attr].mType);

	attr++;
	list[attr].mAttr = GX_VA_NULL;
}

/**
 * @note Address: 0x800E46D0
 * @note Size: 0x38
 */
void GXClearVtxDesc(void)
{
	gx->vcdLo = 0;
	GX_SET_REG(gx->vcdLo, GX_DIRECT, GX_CP_VCD_LO_POS_ST, GX_CP_VCD_LO_POS_END);
	gx->vcdHi     = 0;
	gx->hasNrms   = FALSE;
	gx->hasBiNrms = FALSE;
	gx->dirtyState |= GX_DIRTY_VCD;
}

static inline void SETVAT(u32* vatA, u32* vatB, u32* vatC, GXAttr attr, GXCompCnt compCnt, GXCompType compType, u8 shift)
{
	switch (attr) {
	case GX_VA_POS:
		GX_SET_REG(*vatA, compCnt, GX_CP_VAT_GRP0_POS_CNT_ST, GX_CP_VAT_GRP0_POS_CNT_END);
		GX_SET_REG(*vatA, compType, GX_CP_VAT_GRP0_POS_TYPE_ST, GX_CP_VAT_GRP0_POS_TYPE_END);
		GX_SET_REG(*vatA, shift, GX_CP_VAT_GRP0_POS_SHIFT_ST, GX_CP_VAT_GRP0_POS_SHIFT_END);
		break;
	case GX_VA_NRM:
	case GX_VA_NBT:
		GX_SET_REG(*vatA, compType, GX_CP_VAT_GRP0_NRM_TYPE_ST, GX_CP_VAT_GRP0_NRM_TYPE_END);
		if (compCnt == GX_NRM_NBT3) {
			// Probably because the compCnt can only be one bit?
			GX_SET_REG(*vatA, GX_NRM_NBT, GX_CP_VAT_GRP0_NRM_CNT_ST, GX_CP_VAT_GRP0_NRM_CNT_END);
			GX_SET_REG(*vatA, TRUE, GX_CP_VAT_GRP0_NRMIDX3_ST, GX_CP_VAT_GRP0_NRMIDX3_END);
		} else {
			GX_SET_REG(*vatA, compCnt, GX_CP_VAT_GRP0_NRM_CNT_ST, GX_CP_VAT_GRP0_NRM_CNT_END);
			GX_SET_REG(*vatA, FALSE, GX_CP_VAT_GRP0_NRMIDX3_ST, GX_CP_VAT_GRP0_NRMIDX3_END);
		}
		break;
	case GX_VA_CLR0:
		GX_SET_REG(*vatA, compCnt, GX_CP_VAT_GRP0_CLRDIFF_CNT_ST, GX_CP_VAT_GRP0_CLRDIFF_CNT_END);
		GX_SET_REG(*vatA, compType, GX_CP_VAT_GRP0_CLRDIFF_TYPE_ST, GX_CP_VAT_GRP0_CLRDIFF_TYPE_END);
		break;
	case GX_VA_CLR1:
		GX_SET_REG(*vatA, compCnt, GX_CP_VAT_GRP0_CLRSPEC_CNT_ST, GX_CP_VAT_GRP0_CLRSPEC_CNT_END);
		GX_SET_REG(*vatA, compType, GX_CP_VAT_GRP0_CLRSPEC_TYPE_ST, GX_CP_VAT_GRP0_CLRSPEC_TYPE_END);
		break;
	case GX_VA_TEX0:
		GX_SET_REG(*vatA, compCnt, GX_CP_VAT_GRP0_TXC0_CNT_ST, GX_CP_VAT_GRP0_TXC0_CNT_END);
		GX_SET_REG(*vatA, compType, GX_CP_VAT_GRP0_TXC0_TYPE_ST, GX_CP_VAT_GRP0_TXC0_TYPE_END);
		GX_SET_REG(*vatA, shift, GX_CP_VAT_GRP0_TXC0_SHIFT_ST, GX_CP_VAT_GRP0_TXC0_SHIFT_END);
		break;
	case GX_VA_TEX1:
		GX_SET_REG(*vatB, compCnt, GX_CP_VAT_GRP1_TXC1_CNT_ST, GX_CP_VAT_GRP1_TXC1_CNT_END);
		GX_SET_REG(*vatB, compType, GX_CP_VAT_GRP1_TXC1_TYPE_ST, GX_CP_VAT_GRP1_TXC1_TYPE_END);
		GX_SET_REG(*vatB, shift, GX_CP_VAT_GRP1_TXC1_SHIFT_ST, GX_CP_VAT_GRP1_TXC1_SHIFT_END);
		break;
	case GX_VA_TEX2:
		GX_SET_REG(*vatB, compCnt, GX_CP_VAT_GRP1_TXC2_CNT_ST, GX_CP_VAT_GRP1_TXC2_CNT_END);
		GX_SET_REG(*vatB, compType, GX_CP_VAT_GRP1_TXC2_TYPE_ST, GX_CP_VAT_GRP1_TXC2_TYPE_END);
		GX_SET_REG(*vatB, shift, GX_CP_VAT_GRP1_TXC2_SHIFT_ST, GX_CP_VAT_GRP1_TXC2_SHIFT_END);
		break;
	case GX_VA_TEX3:
		GX_SET_REG(*vatB, compCnt, GX_CP_VAT_GRP1_TXC3_CNT_ST, GX_CP_VAT_GRP1_TXC3_CNT_END);
		GX_SET_REG(*vatB, compType, GX_CP_VAT_GRP1_TXC3_TYPE_ST, GX_CP_VAT_GRP1_TXC3_TYPE_END);
		GX_SET_REG(*vatB, shift, GX_CP_VAT_GRP1_TXC3_SHIFT_ST, GX_CP_VAT_GRP1_TXC3_SHIFT_END);
		break;
	case GX_VA_TEX4:
		GX_SET_REG(*vatB, compCnt, GX_CP_VAT_GRP1_TXC4_CNT_ST, GX_CP_VAT_GRP1_TXC4_CNT_END);
		GX_SET_REG(*vatB, compType, GX_CP_VAT_GRP1_TXC4_TYPE_ST, GX_CP_VAT_GRP1_TXC4_TYPE_END);
		GX_SET_REG(*vatC, shift, GX_CP_VAT_GRP2_TXC4_SHIFT_ST, GX_CP_VAT_GRP2_TXC4_SHIFT_END);
		break;
	case GX_VA_TEX5:
		GX_SET_REG(*vatC, compCnt, GX_CP_VAT_GRP2_TXC5_CNT_ST, GX_CP_VAT_GRP2_TXC5_CNT_END);
		GX_SET_REG(*vatC, compType, GX_CP_VAT_GRP2_TXC5_TYPE_ST, GX_CP_VAT_GRP2_TXC5_TYPE_END);
		GX_SET_REG(*vatC, shift, GX_CP_VAT_GRP2_TXC5_SHIFT_ST, GX_CP_VAT_GRP2_TXC5_SHIFT_END);
		break;
	case GX_VA_TEX6:
		GX_SET_REG(*vatC, compCnt, GX_CP_VAT_GRP2_TXC6_CNT_ST, GX_CP_VAT_GRP2_TXC6_CNT_END);
		GX_SET_REG(*vatC, compType, GX_CP_VAT_GRP2_TXC6_TYPE_ST, GX_CP_VAT_GRP2_TXC6_TYPE_END);
		GX_SET_REG(*vatC, shift, GX_CP_VAT_GRP2_TXC6_SHIFT_ST, GX_CP_VAT_GRP2_TXC6_SHIFT_END);
		break;
	case GX_VA_TEX7:
		GX_SET_REG(*vatC, compCnt, GX_CP_VAT_GRP2_TXC7_CNT_ST, GX_CP_VAT_GRP2_TXC7_CNT_END);
		GX_SET_REG(*vatC, compType, GX_CP_VAT_GRP2_TXC7_TYPE_ST, GX_CP_VAT_GRP2_TXC7_TYPE_END);
		GX_SET_REG(*vatC, shift, GX_CP_VAT_GRP2_TXC7_SHIFT_ST, GX_CP_VAT_GRP2_TXC7_SHIFT_END);
		break;
	}
}

/**
 * @note Address: 0x800E4708
 * @note Size: 0x25C
 */
void GXSetVtxAttrFmt(GXVtxFmt format, GXAttr attr, GXCompCnt count, GXCompType type, u8 frac)
{
	u32* vA = &gx->vatA[format];
	u32* vB = &gx->vatB[format];
	u32* vC = &gx->vatC[format];

	SETVAT(vA, vB, vC, attr, count, type, frac);

	gx->dirtyState |= GX_DIRTY_VAT;
	gx->dirtyVAT |= (u8)(1 << (u8)format);
}

/**
 * @note Address: 0x800E4964
 * @note Size: 0x280
 */
void GXSetVtxAttrFmtv(GXVtxFmt format, GXVtxAttrFmtList* list)
{
	u32* vatA;
	u32* vatB;
	u32* vatC;

	vatA = &gx->vatA[format];
	vatB = &gx->vatB[format];
	vatC = &gx->vatC[format];

	for (; list->mAttr != GX_VA_NULL; list++) {
		SETVAT(vatA, vatB, vatC, list->mAttr, list->mCount, list->mType, list->mFrac);
	}

	gx->dirtyState |= GX_DIRTY_VAT;
	gx->dirtyVAT |= (u8)(1 << (u8)format);
}

/**
 * @note Address: 0x800E4BE4
 * @note Size: 0x9C
 */
void __GXSetVAT(void)
{
	int i = 0;
	u32 dirtyVAT = gx->dirtyVAT;
	do
	{
		if (dirtyVAT & 1)
		{
			GX_CP_LOAD_REG(GX_CP_REG_VAT_GRP0 | i, gx->vatA[i]);
			GX_CP_LOAD_REG(GX_CP_REG_VAT_GRP1 | i, gx->vatB[i]);
			GX_CP_LOAD_REG(GX_CP_REG_VAT_GRP2 | i, gx->vatC[i]);
		}
	} while (i++, dirtyVAT >>= 1);

	gx->dirtyVAT = 0;
}

static u8 GetFracForNrm(GXCompType compType) {
    switch (compType) {
    case GX_S8:
        return 6;
    case GX_S16:
        return 14;
    case GX_U16:
    default:
        return 0;
    }
}

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* compCnt,
                     GXCompType* compType, u8* shift) {
    u32* vatA;
    u32* vatB;
    u32* vatC;

    vatA = &gx->vatA[fmt];
    vatB = &gx->vatB[fmt];
    vatC = &gx->vatC[fmt];

    switch(attr) {
    case GX_VA_POS:
        *compCnt = (GXCompCnt)(*vatA & 1);
        *compType = (GXCompType)(*vatA >> 1 & 7);
        *shift = *vatA >> 4 & 0x1F;
        break;
    case GX_VA_NRM:
    case GX_VA_NBT:
        *compCnt = (GXCompCnt)(*vatA >> 9 & 1);
        // Check NormalIndex3 field of VAT_A
        if (*compCnt == GX_NRM_NBT && (*vatA >> 31 & 1)) {
            *compCnt = GX_NRM_NBT3;
        }
        *compType = (GXCompType)(*vatA >> 10 & 7);
        *shift = GetFracForNrm(*compType);
        break;
    case GX_VA_CLR0:
        *compCnt = (GXCompCnt)(*vatA >> 13 & 1);
        *compType = (GXCompType)(*vatA >> 14 & 7);
        *shift = 0;
        break;
    case GX_VA_CLR1:
        *compCnt = (GXCompCnt)(*vatA >> 17 & 1);
        *compType = (GXCompType)(*vatA >> 18 & 7);
        *shift = 0;
        break;
    case GX_VA_TEX0:
        *compCnt = (GXCompCnt)(*vatA >> 21 & 1);
        *compType = (GXCompType)(*vatA >> 22 & 7);
        *shift = *vatA >> 25 & 0x1F;
        break;
    case GX_VA_TEX1:
        *compCnt = (GXCompCnt)(*vatB >> 0 & 1);
        *compType = (GXCompType)(*vatB >> 1 & 7);
        *shift = *vatB >> 4 & 0x1F;
        break;
    case GX_VA_TEX2:
        *compCnt = (GXCompCnt)(*vatB >> 9 & 1);
        *compType = (GXCompType)(*vatB >> 10 & 7);
        *shift = *vatB >> 13 & 0x1F;
        break;
    case GX_VA_TEX3:
        *compCnt = (GXCompCnt)(*vatB >> 18 & 1);
        *compType = (GXCompType)(*vatB >> 19 & 7);
        *shift = *vatB >> 22 & 0x1F;
        break;
    case GX_VA_TEX4:
        *compCnt = (GXCompCnt)(*vatB >> 27 & 1);
        *compType = (GXCompType)(*vatB >> 28 & 7);
        *shift = *vatC >> 0 & 0x1F;
        break;
    case GX_VA_TEX5:
        *compCnt = (GXCompCnt)(*vatC >> 5 & 1);
        *compType = (GXCompType)(*vatC >> 6 & 7);
        *shift = *vatC >> 9 & 0x1F;
        break;
    case GX_VA_TEX6:
        *compCnt = (GXCompCnt)(*vatC >> 14 & 1);
        *compType = (GXCompType)(*vatC >> 15 & 7);
        *shift = *vatC >> 18 & 0x1F;
        break;
    case GX_VA_TEX7:
        *compCnt = (GXCompCnt)(*vatC >> 23 & 1);
        *compType = (GXCompType)(*vatC >> 24 & 7);
        *shift = *vatC >> 27 & 0x1F;
        break;
    default:
        *compCnt = GX_POS_XYZ;
        *compType = GX_U8;
        *shift = 0;
    }
}

/**
 * @note Address: N/A
 * @note Size: 0x74
 */
void GXGetVtxAttrFmtv(GXVtxFmt format, GXVtxAttrFmtList* list)
{
	GXAttr attr = GX_VA_POS;
	do {
		list->mAttr = attr;
		GXGetVtxAttrFmt(format, attr, &list->mCount, &list->mType, &list->mFrac);
		attr += 1;
		list++;
	} while(attr <= GX_VA_TEX7);
	list->mAttr = GX_VA_NULL;
}

/**
 * @note Address: 0x800E4C80
 * @note Size: 0x8C
 */
void GXSetArray(GXAttr attr, void* basePtr, u8 stride)
{
	s32 newAttr;
	s32 attrReg;

	newAttr = attr;
	if (newAttr == GX_VA_NBT)
	{
		newAttr = GX_VA_NRM;
	}

	attrReg = newAttr - GX_VA_POS;

	GX_CP_LOAD_REG(GX_BP_REG_SETMODE0_TEX4 | attrReg, (size_t)basePtr & ~0xC0000000);
	GX_CP_LOAD_REG(GX_BP_REG_SETIMAGE2_TEX4 | attrReg, stride);
}

/**
 * @note Address: 0x800E4D0C
 * @note Size: 0x10
 */
void GXInvalidateVtxCache(void) { GX_WRITE_U8(GX_FIFO_CMD_INVAL_VTX); }

/**
 * @note Address: 0x800E4D1C
 * @note Size: 0x280
 */
void GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType type, GXTexGenSrc src, u32 texMtxIdx, GXBool normalize, u32 dualTexMtxIdx)
{
	u32 reg;
	u32 inputRow;
	GXXfTexReg inputForm;

	reg       = 0;
	inputForm = GX_XF_TEX_FORM_AB11;
	inputRow  = 5;

	switch (src) {
	case GX_TG_POS:
		inputRow  = 0;
		inputForm = GX_XF_TEX_FORM_ABC1;
		break;
	case GX_TG_NRM:
		inputRow  = 1;
		inputForm = GX_XF_TEX_FORM_ABC1;
		break;
	case GX_TG_BINRM:
		inputRow  = 3;
		inputForm = GX_XF_TEX_FORM_ABC1;
		break;
	case GX_TG_TANGENT:
		inputRow  = 4;
		inputForm = GX_XF_TEX_FORM_ABC1;
		break;
	case GX_TG_COLOR0:
		inputRow = 2;
		break;
	case GX_TG_COLOR1:
		inputRow = 2;
		break;
	case GX_TG_TEX0:
		inputRow = 5;
		break;
	case GX_TG_TEX1:
		inputRow = 6;
		break;
	case GX_TG_TEX2:
		inputRow = 7;
		break;
	case GX_TG_TEX3:
		inputRow = 8;
		break;
	case GX_TG_TEX4:
		inputRow = 9;
		break;
	case GX_TG_TEX5:
		inputRow = 10;
		break;
	case GX_TG_TEX6:
		inputRow = 11;
		break;
	case GX_TG_TEX7:
		inputRow = 12;
		break;
	}

	switch (type) {
	case GX_TG_NRM:
		// reg = 0;
		GX_SET_REG(reg, GX_XF_TEX_PROJ_ST, GX_XF_TEX_PROJTYPE_ST, GX_XF_TEX_PROJTYPE_END); // 2x4 projection
		GX_SET_REG(reg, inputForm, GX_XF_TEX_INPUTFORM_ST, GX_XF_TEX_INPUTFORM_END);
		GX_SET_REG(reg, GX_TG_POS, GX_XF_TEX_TEXGENTYPE_ST, GX_XF_TEX_TEXGENTYPE_END);
		GX_SET_REG(reg, inputRow, GX_XF_TEX_SRCROW_ST, GX_XF_TEX_SRCROW_END);
		break;
	case GX_TG_POS:
		// reg = 0;
		GX_SET_REG(reg, GX_XF_TEX_PROJ_STQ, GX_XF_TEX_PROJTYPE_ST, GX_XF_TEX_PROJTYPE_END); // 3x4 projection
		GX_SET_REG(reg, inputForm, GX_XF_TEX_INPUTFORM_ST, GX_XF_TEX_INPUTFORM_END);
		GX_SET_REG(reg, GX_TG_POS, GX_XF_TEX_TEXGENTYPE_ST, GX_XF_TEX_TEXGENTYPE_END);
		GX_SET_REG(reg, inputRow, GX_XF_TEX_SRCROW_ST, GX_XF_TEX_SRCROW_END);
		break;
	case GX_TG_BUMP0:
	case GX_TG_BUMP1:
	case GX_TG_BUMP2:
	case GX_TG_BUMP3:
	case GX_TG_BUMP4:
	case GX_TG_BUMP5:
	case GX_TG_BUMP6:
	case GX_TG_BUMP7:
		// reg = 0;
		GX_SET_REG(reg, GX_XF_TEX_PROJ_ST, GX_XF_TEX_PROJTYPE_ST, GX_XF_TEX_PROJTYPE_END); // 2x4 projection
		GX_SET_REG(reg, inputForm, GX_XF_TEX_INPUTFORM_ST, GX_XF_TEX_INPUTFORM_END);
		GX_SET_REG(reg, GX_TG_NRM, GX_XF_TEX_TEXGENTYPE_ST, GX_XF_TEX_TEXGENTYPE_END);
		GX_SET_REG(reg, inputRow, GX_XF_TEX_SRCROW_ST, GX_XF_TEX_SRCROW_END);
		GX_SET_REG(reg, src - GX_TG_TEXCOORD0, GX_XF_TEX_BUMPSRCTEX_ST, GX_XF_TEX_BUMPSRCTEX_END);
		GX_SET_REG(reg, type - GX_TG_BUMP0, GX_XF_TEX_BUMPSRCLIGHT_ST, GX_XF_TEX_BUMPSRCLIGHT_END);
		break;
	case GX_TG_SRTG:
		// reg = 0;
		GX_SET_REG(reg, GX_XF_TEX_PROJ_ST, GX_XF_TEX_PROJTYPE_ST, GX_XF_TEX_PROJTYPE_END); // 2x4 projection
		GX_SET_REG(reg, inputForm, GX_XF_TEX_INPUTFORM_ST, GX_XF_TEX_INPUTFORM_END);

		if (src == GX_TG_COLOR0) {
			GX_SET_REG(reg, GX_XF_TG_CLR0, GX_XF_TEX_TEXGENTYPE_ST, GX_XF_TEX_TEXGENTYPE_END);
		} else {
			GX_SET_REG(reg, GX_XF_TG_CLR1, GX_XF_TEX_TEXGENTYPE_ST, GX_XF_TEX_TEXGENTYPE_END);
		}
		GX_SET_REG(reg, 2, GX_XF_TEX_SRCROW_ST, GX_XF_TEX_SRCROW_END);
		break;
	default:
		break;
	}

	GX_XF_LOAD_REG(GX_XF_REG_TEX0 + id, reg);

	reg = 0;
	GX_SET_REG(reg, dualTexMtxIdx - 0x40, GX_XF_MTXIDX0_GEOM_ST, GX_XF_MTXIDX0_GEOM_END);
	GX_SET_REG(reg, normalize, GX_XF_DUALTEX_NORMALISE_ST, GX_XF_DUALTEX_NORMALISE_END);

	GX_XF_LOAD_REG(GX_XF_REG_DUALTEX0 + id, reg);

	switch (id) {
	case GX_TEXCOORD0:
		GX_SET_REG(gx->matIdxA, texMtxIdx, GX_XF_MTXIDX0_TEX0_ST, GX_XF_MTXIDX0_TEX0_END);
		break;
	case GX_TEXCOORD1:
		GX_SET_REG(gx->matIdxA, texMtxIdx, GX_XF_MTXIDX0_TEX1_ST, GX_XF_MTXIDX0_TEX1_END);
		break;
	case GX_TEXCOORD2:
		GX_SET_REG(gx->matIdxA, texMtxIdx, GX_XF_MTXIDX0_TEX2_ST, GX_XF_MTXIDX0_TEX2_END);
		break;
	case GX_TEXCOORD3:
		GX_SET_REG(gx->matIdxA, texMtxIdx, GX_XF_MTXIDX0_TEX3_ST, GX_XF_MTXIDX0_TEX3_END);
		break;
	case GX_TEXCOORD4:
		GX_SET_REG(gx->matIdxB, texMtxIdx, GX_XF_MTXIDX1_TEX4_ST, GX_XF_MTXIDX1_TEX4_END);
		break;
	case GX_TEXCOORD5:
		GX_SET_REG(gx->matIdxB, texMtxIdx, GX_XF_MTXIDX1_TEX5_ST, GX_XF_MTXIDX1_TEX5_END);
		break;
	case GX_TEXCOORD6:
		GX_SET_REG(gx->matIdxB, texMtxIdx, GX_XF_MTXIDX1_TEX6_ST, GX_XF_MTXIDX1_TEX6_END);
		break;
	default:
		GX_SET_REG(gx->matIdxB, texMtxIdx, GX_XF_MTXIDX1_TEX7_ST, GX_XF_MTXIDX1_TEX7_END);
		break;
	}
	__GXSetMatrixIndex(id + 1);
}

/**
 * @note Address: 0x800E4F9C
 * @note Size: 0x3C
 */
void GXSetNumTexGens(u8 count)
{
	GX_SET_REG(gx->genMode, count, GX_BP_GENMODE_NUMTEX_ST, GX_BP_GENMODE_NUMTEX_END);
	GX_XF_LOAD_REG(GX_XF_REG_NUMTEX, count);
	gx->dirtyState |= GX_DIRTY_GEN_MODE;
}
