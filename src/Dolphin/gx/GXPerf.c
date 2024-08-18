#include "Dolphin/gx.h"
#include "Dolphin/GX/GXData.h"

/**
 * @note Address: 0x800E9984
 * @note Size: 0x848
 */
void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1)
{
	switch (gx->perf0) {
	case GX_PERF0_VERTICES:
	case GX_PERF0_CLIP_VTX:
	case GX_PERF0_CLIP_CLKS:
	case GX_PERF0_XF_WAIT_IN:
	case GX_PERF0_XF_WAIT_OUT:
	case GX_PERF0_XF_XFRM_CLKS:
	case GX_PERF0_XF_LIT_CLKS:
	case GX_PERF0_XF_BOT_CLKS:
	case GX_PERF0_XF_REGLD_CLKS:
	case GX_PERF0_XF_REGRD_CLKS:
	case GX_PERF0_CLIP_RATIO:
	case GX_PERF0_CLOCKS:
		GX_XF_LOAD_REG(0x1006, 0);
		break;

	case GX_PERF0_TRIANGLES:
	case GX_PERF0_TRIANGLES_CULLED:
	case GX_PERF0_TRIANGLES_PASSED:
	case GX_PERF0_TRIANGLES_SCISSORED:
	case GX_PERF0_TRIANGLES_0TEX:
	case GX_PERF0_TRIANGLES_1TEX:
	case GX_PERF0_TRIANGLES_2TEX:
	case GX_PERF0_TRIANGLES_3TEX:
	case GX_PERF0_TRIANGLES_4TEX:
	case GX_PERF0_TRIANGLES_5TEX:
	case GX_PERF0_TRIANGLES_6TEX:
	case GX_PERF0_TRIANGLES_7TEX:
	case GX_PERF0_TRIANGLES_8TEX:
	case GX_PERF0_TRIANGLES_0CLR:
	case GX_PERF0_TRIANGLES_1CLR:
	case GX_PERF0_TRIANGLES_2CLR:
		GX_BP_LOAD_REG(0x23000000);
		break;

	case GX_PERF0_QUAD_0CVG:
	case GX_PERF0_QUAD_NON0CVG:
	case GX_PERF0_QUAD_1CVG:
	case GX_PERF0_QUAD_2CVG:
	case GX_PERF0_QUAD_3CVG:
	case GX_PERF0_QUAD_4CVG:
	case GX_PERF0_AVG_QUAD_CNT:
		GX_BP_LOAD_REG(0x24000000);
		break;

	case GX_PERF0_NONE:
		break;
	}

	switch (gx->perf1) {
	case GX_PERF1_TEXELS:
	case GX_PERF1_TX_IDLE:
	case GX_PERF1_TX_REGS:
	case GX_PERF1_TX_MEMSTALL:
	case GX_PERF1_TC_CHECK1_2:
	case GX_PERF1_TC_CHECK3_4:
	case GX_PERF1_TC_CHECK5_6:
	case GX_PERF1_TC_CHECK7_8:
	case GX_PERF1_TC_MISS:
	case GX_PERF1_CLOCKS:
		GX_BP_LOAD_REG(0x67000000);
		break;

	case GX_PERF1_VC_ELEMQ_FULL:
	case GX_PERF1_VC_MISSQ_FULL:
	case GX_PERF1_VC_MEMREQ_FULL:
	case GX_PERF1_VC_STATUS7:
	case GX_PERF1_VC_MISSREP_FULL:
	case GX_PERF1_VC_STREAMBUF_LOW:
	case GX_PERF1_VC_ALL_STALLS:
	case GX_PERF1_VERTICES:
		GX_SET_REG(gx->perfSel, 0, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;

	case GX_PERF1_FIFO_REQ:
	case GX_PERF1_CALL_REQ:
	case GX_PERF1_VC_MISS_REQ:
	case GX_PERF1_CP_ALL_REQ:
		GX_SET_CP_REG(3, 0);
		break;

	case GX_PERF1_NONE:
		break;
	}

	gx->perf0 = perf0;

	switch (gx->perf0) {
	case GX_PERF0_VERTICES:
		GX_XF_LOAD_REG(0x1006, 0x273);
		break;
	case GX_PERF0_CLIP_VTX:
		GX_XF_LOAD_REG(0x1006, 0x14A);
		break;
	case GX_PERF0_CLIP_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x16B);
		break;
	case GX_PERF0_XF_WAIT_IN:
		GX_XF_LOAD_REG(0x1006, 0x84);
		break;
	case GX_PERF0_XF_WAIT_OUT:
		GX_XF_LOAD_REG(0x1006, 0xC6);
		break;
	case GX_PERF0_XF_XFRM_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x210);
		break;
	case GX_PERF0_XF_LIT_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x252);
		break;
	case GX_PERF0_XF_BOT_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x231);
		break;
	case GX_PERF0_XF_REGLD_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x1AD);
		break;
	case GX_PERF0_XF_REGRD_CLKS:
		GX_XF_LOAD_REG(0x1006, 0x1CE);
		break;
	case GX_PERF0_CLOCKS:
		GX_XF_LOAD_REG(0x1006, 0x21);
		break;
	case GX_PERF0_CLIP_RATIO:
		GX_XF_LOAD_REG(0x1006, 0x153);
		break;

	case GX_PERF0_TRIANGLES:
		GX_BP_LOAD_REG(0x2300AE7F);
		break;
	case GX_PERF0_TRIANGLES_CULLED:
		GX_BP_LOAD_REG(0x23008E7F);
		break;
	case GX_PERF0_TRIANGLES_PASSED:
		GX_BP_LOAD_REG(0x23009E7F);
		break;
	case GX_PERF0_TRIANGLES_SCISSORED:
		GX_BP_LOAD_REG(0x23001E7F);
		break;
	case GX_PERF0_TRIANGLES_0TEX:
		GX_BP_LOAD_REG(0x2300AC3F);
		break;
	case GX_PERF0_TRIANGLES_1TEX:
		GX_BP_LOAD_REG(0x2300AC7F);
		break;
	case GX_PERF0_TRIANGLES_2TEX:
		GX_BP_LOAD_REG(0x2300ACBF);
		break;
	case GX_PERF0_TRIANGLES_3TEX:
		GX_BP_LOAD_REG(0x2300ACFF);
		break;
	case GX_PERF0_TRIANGLES_4TEX:
		GX_BP_LOAD_REG(0x2300AD3F);
		break;
	case GX_PERF0_TRIANGLES_5TEX:
		GX_BP_LOAD_REG(0x2300AD7F);
		break;
	case GX_PERF0_TRIANGLES_6TEX:
		GX_BP_LOAD_REG(0x2300ADBF);
		break;
	case GX_PERF0_TRIANGLES_7TEX:
		GX_BP_LOAD_REG(0x2300ADFF);
		break;
	case GX_PERF0_TRIANGLES_8TEX:
		GX_BP_LOAD_REG(0x2300AE3F);
		break;
	case GX_PERF0_TRIANGLES_0CLR:
		GX_BP_LOAD_REG(0x2300A27F);
		break;
	case GX_PERF0_TRIANGLES_1CLR:
		GX_BP_LOAD_REG(0x2300A67F);
		break;
	case GX_PERF0_TRIANGLES_2CLR:
		GX_BP_LOAD_REG(0x2300AA7F);
		break;

	case GX_PERF0_QUAD_0CVG:
		GX_BP_LOAD_REG(0x2402C0C6);
		break;
	case GX_PERF0_QUAD_NON0CVG:
		GX_BP_LOAD_REG(0x2402C16B);
		break;
	case GX_PERF0_QUAD_1CVG:
		GX_BP_LOAD_REG(0x2402C0E7);
		break;
	case GX_PERF0_QUAD_2CVG:
		GX_BP_LOAD_REG(0x2402C108);
		break;
	case GX_PERF0_QUAD_3CVG:
		GX_BP_LOAD_REG(0x2402C129);
		break;
	case GX_PERF0_QUAD_4CVG:
		GX_BP_LOAD_REG(0x2402C14A);
		break;
	case GX_PERF0_AVG_QUAD_CNT:
		GX_BP_LOAD_REG(0x2402C1AD);
		break;

	case GX_PERF0_NONE:
		break;
	}

	gx->perf1 = perf1;

	switch (gx->perf1) {
	case GX_PERF1_TEXELS:
		GX_BP_LOAD_REG(0x67000042);
		break;
	case GX_PERF1_TX_IDLE:
		GX_BP_LOAD_REG(0x67000084);
		break;
	case GX_PERF1_TX_REGS:
		GX_BP_LOAD_REG(0x67000063);
		break;
	case GX_PERF1_TX_MEMSTALL:
		GX_BP_LOAD_REG(0x67000129);
		break;
	case GX_PERF1_TC_MISS:
		GX_BP_LOAD_REG(0x67000252);
		break;
	case GX_PERF1_CLOCKS:
		GX_BP_LOAD_REG(0x67000021);
		break;
	case GX_PERF1_TC_CHECK1_2:
		GX_BP_LOAD_REG(0x6700014B);
		break;
	case GX_PERF1_TC_CHECK3_4:
		GX_BP_LOAD_REG(0x6700018D);
		break;
	case GX_PERF1_TC_CHECK5_6:
		GX_BP_LOAD_REG(0x670001CF);
		break;
	case GX_PERF1_TC_CHECK7_8:
		GX_BP_LOAD_REG(0x67000211);
		break;

	case GX_PERF1_VC_ELEMQ_FULL:
		GX_SET_REG(gx->perfSel, 2, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_MISSQ_FULL:
		GX_SET_REG(gx->perfSel, 3, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_MEMREQ_FULL:
		GX_SET_REG(gx->perfSel, 4, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_STATUS7:
		GX_SET_REG(gx->perfSel, 5, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_MISSREP_FULL:
		GX_SET_REG(gx->perfSel, 6, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_STREAMBUF_LOW:
		GX_SET_REG(gx->perfSel, 7, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VC_ALL_STALLS:
		GX_SET_REG(gx->perfSel, 9, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;
	case GX_PERF1_VERTICES:
		GX_SET_REG(gx->perfSel, 8, 24, 27);
		GX_CP_LOAD_REG(0x20, gx->perfSel);
		break;

	case GX_PERF1_FIFO_REQ:
		GX_SET_CP_REG(3, 2);
		break;
	case GX_PERF1_CALL_REQ:
		GX_SET_CP_REG(3, 3);
		break;
	case GX_PERF1_VC_MISS_REQ:
		GX_SET_CP_REG(3, 4);
		break;
	case GX_PERF1_CP_ALL_REQ:
		GX_SET_CP_REG(3, 5);
		break;

	case GX_PERF1_NONE:
		break;
	}

	gx->bpSentNot = GX_FALSE;
}

/**
 * @note Address: 0x800EA1DC
 * @note Size: 0xC4
 * TODO: use enums for the read regs
 */
#pragma scheduling off
void GXReadXfRasMetric(u32* xfWaitIn, u32* xfWaitOut, u32* rasBusy, u32* clocks)
{
	*rasBusy   = GXReadCPReg(32, 33);
	*clocks    = GXReadCPReg(34, 35);
	*xfWaitIn  = GXReadCPReg(36, 37);
	*xfWaitOut = GXReadCPReg(38, 39);
}
#pragma scheduling reset

#pragma scheduling off
void GXReadGPMetric(u32 *cnt0, u32 *cnt1)
{
	u32 xfWaitIn, xfWaitOut, rasBusy, clocks;

	GXReadXfRasMetric(&xfWaitIn, &xfWaitOut, &rasBusy, &clocks);

	switch (gx->perf0)
	{
	case GX_PERF0_CLIP_RATIO:
		*cnt0 = clocks * 1000 / rasBusy;
		break;
	case GX_PERF0_VERTICES:
	case GX_PERF0_CLIP_VTX:
	case GX_PERF0_CLIP_CLKS:
	case GX_PERF0_XF_WAIT_IN:
	case GX_PERF0_XF_WAIT_OUT:
	case GX_PERF0_XF_XFRM_CLKS:
	case GX_PERF0_XF_LIT_CLKS:
	case GX_PERF0_XF_BOT_CLKS:
	case GX_PERF0_XF_REGLD_CLKS:
	case GX_PERF0_XF_REGRD_CLKS:

	case GX_PERF0_TRIANGLES:
	case GX_PERF0_TRIANGLES_CULLED:
	case GX_PERF0_TRIANGLES_PASSED:
	case GX_PERF0_TRIANGLES_SCISSORED:
	case GX_PERF0_TRIANGLES_0TEX:
	case GX_PERF0_TRIANGLES_1TEX:
	case GX_PERF0_TRIANGLES_2TEX:
	case GX_PERF0_TRIANGLES_3TEX:
	case GX_PERF0_TRIANGLES_4TEX:
	case GX_PERF0_TRIANGLES_5TEX:
	case GX_PERF0_TRIANGLES_6TEX:
	case GX_PERF0_TRIANGLES_7TEX:
	case GX_PERF0_TRIANGLES_8TEX:
	case GX_PERF0_TRIANGLES_0CLR:
	case GX_PERF0_TRIANGLES_1CLR:
	case GX_PERF0_TRIANGLES_2CLR:
	case GX_PERF0_QUAD_0CVG:
	case GX_PERF0_QUAD_NON0CVG:
	case GX_PERF0_QUAD_1CVG:
	case GX_PERF0_QUAD_2CVG:
	case GX_PERF0_QUAD_3CVG:
	case GX_PERF0_QUAD_4CVG:
	case GX_PERF0_AVG_QUAD_CNT:
	case GX_PERF0_CLOCKS:
		*cnt0 = rasBusy;
		break;
	case GX_PERF0_NONE:
		*cnt0 = 0;
		break;
	default:
		*cnt0 = 0;
		break;
	}

	switch (gx->perf1)
	{
	case GX_PERF1_TEXELS:
		*cnt1 = (xfWaitIn * 0) + (xfWaitOut * 4);
		break;
	case GX_PERF1_TC_CHECK1_2:
		*cnt1 = (xfWaitIn * 1) + (xfWaitOut * 2);
		break;
	case GX_PERF1_TC_CHECK3_4:
		*cnt1 = (xfWaitIn * 3) + (xfWaitOut * 4);
		break;
	case GX_PERF1_TC_CHECK5_6:
		*cnt1 = (xfWaitIn * 5) + (xfWaitOut * 6);
		break;
	case GX_PERF1_TC_CHECK7_8:
		*cnt1 = (xfWaitIn * 7) + (xfWaitOut * 8);
		break;
	case GX_PERF1_TX_IDLE:
	case GX_PERF1_TX_REGS:
	case GX_PERF1_TX_MEMSTALL:
	case GX_PERF1_TC_MISS:
	case GX_PERF1_VC_ELEMQ_FULL:
	case GX_PERF1_VC_MISSQ_FULL:
	case GX_PERF1_VC_MEMREQ_FULL:
	case GX_PERF1_VC_STATUS7:
	case GX_PERF1_VC_MISSREP_FULL:
	case GX_PERF1_VC_STREAMBUF_LOW:
	case GX_PERF1_VC_ALL_STALLS:
	case GX_PERF1_VERTICES:
	case GX_PERF1_CLOCKS:
		*cnt1 = xfWaitOut;
		break;
	case GX_PERF1_FIFO_REQ:
	case GX_PERF1_CALL_REQ:
	case GX_PERF1_VC_MISS_REQ:
	case GX_PERF1_CP_ALL_REQ:
		*cnt1 = xfWaitIn;
		break;
	case GX_PERF1_NONE:
		*cnt1 = 0;
		break;
	default:
		*cnt1 = 0;
		break;
	}
}
#pragma scheduling reset

/**
 * @note Address: 0x800EA1CC
 * @note Size: 0x10
 */
void GXClearGPMetric(void) { GX_SET_CP_REG(2, 4); }

/**
 * @note Address: N/A
 * @note Size: 0x2C
 */
u32 GXReadGP0Metric(void)
{
	u32 a, b;
	GXReadGPMetric(&a, &b);
	return a;
}

/**
 * @note Address: N/A
 * @note Size: 0x2C
 */
u32 GXReadGP1Metric(void)
{
	u32 a, b;
	GXReadGPMetric(&a, &b);
	return b;
}

/**
 * @note Address: N/A
 * @note Size: 0x214
 */
void GXReadMemMetric(u32* cpReq, u32* tcReq, u32* cpuReadReq, u32* cpuWriteReq, u32* dspReq, u32* ioReq, u32* viReq, u32* peReq, u32* rfReq,
                     u32* fiReq)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xA8
 */
void GXClearMemMetric(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x138
 */
#pragma scheduling off
void GXReadPixMetric(u32* topIn, u32* topOut, u32* bottomIn, u32* bottomOut, u32* clearIn, u32* copyClocks)
{
	*topIn = GXReadPEReg(12, 13) * 4;
	*topOut = GXReadPEReg(14, 15) * 4;
	*bottomIn = GXReadPEReg(16, 17) * 4;
	*bottomOut = GXReadPEReg(18, 19) * 4;
	*clearIn = GXReadPEReg(20, 21) * 4;
	*copyClocks = GXReadPEReg(22, 23);
}
#pragma scheduling reset

/**
 * @note Address: N/A
 * @note Size: 0x30
 */

void GXClearPixMetric(void)
{
	GX_BP_LOAD_REG(0x57000000);
	GX_BP_LOAD_REG(0x57000aaa);
	gx->bpSentNot = GX_FALSE;
}

/**
 * @note Address: N/A
 * @note Size: 0x44
 */
void GXSetVCacheMetric(GXVCachePerf attr)
{
	GX_BITFIELD_SET(gx->perfSel, 28, 4, attr);
	GX_CP_LOAD_REG(0x20, gx->perfSel);
	GX_CP_LOAD_REG(0x10, 1);
}

/**
 * @note Address: N/A
 * @note Size: 0x94
 */
#pragma scheduling off
void GXReadVCacheMetric(u32* check, u32* miss, u32* stall)
{
	*check = GXReadCPReg(40, 41);
	*miss = GXReadCPReg(42, 43);
	*stall = GXReadCPReg(44, 45);
}
#pragma scheduling reset

/**
 * @note Address: N/A
 * @note Size: 0x1C
 */
void GXClearVCacheMetric(void)
{
	GX_CP_LOAD_REG(0, 0);
}

/**
 * @note Address: N/A
 * @note Size: 0x44
 */
void GXInitXfRasMetric(void)
{
	// UNUSED FUNCTION
}


/**
 * @note Address: N/A
 * @note Size: 0x40
 */
u32 GXReadClksPerVtx(void)
{
	// UNUSED FUNCTION
}
