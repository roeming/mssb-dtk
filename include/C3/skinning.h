#ifndef _DOLPHIN_CP_SKINNING_H_
#define _DOLPHIN_CP_SKINNING_H_

typedef struct _sStats {
    /* 0x00 */ u32 cycles[2];
    /* 0x08 */ u32 loadStores[2];
    /* 0x10 */ u32 missCycles[2];
    /* 0x18 */ u32 instructions[2];
    /* 0x20 */ u32 cycleTotal;
    /* 0x24 */ u32 loadStoreTotal;
    /* 0x28 */ u32 missCycleTotal;
    /* 0x2C */ u32 instructionTotal;
    /* 0x30 */ u32 hits;
} sStats;

typedef struct {
    /* 0x00 */ s16 * src;
    /* 0x04 */ u16 * indices;
    /* 0x08 */ u8 * weights;
} sAccBuffers;

typedef struct sBone {
    /* 0x00 */ u16 boneID;
    /* 0x02 */ u8 inheritanceFlag;
    /* 0x03 */ u8 drawingPriority;
    /* 0x04 */ DSBranch branch;
    /* 0x14 */ struct DODisplayObj * dispObj;
    /* 0x18 */ Control orientationCtrl;
    /* 0x4C */ Control animationCtrl;
    /* 0x80 */ struct ANIMPipe * animPipe;
    /* 0x84 */ MtxPtr forwardMtx;
    /* 0x88 */ MtxPtr skinMtx;
    /* 0x8C */ MtxPtr skinInvTransposeMtx;
    /* 0x90 */ MtxPtr orientationInvMtx;
    /* 0x94 */ DSLink drawPriorityLink;
} sBone;

typedef struct {
    /* 0x00 */ Mtx34 m;
    /* 0x30 */ void * vertSrc;
    /* 0x34 */ void * vertDst;
    /* 0x38 */ u16 boneIndex;
    /* 0x3A */ u16 count;
    /* 0x3C */ u8 vertSrcOffset;
    /* 0x3D */ u8 pad8;
    /* 0x3E */ u16 pad16;
} SK1List;

typedef struct {
    /* 0x00 */ Mtx34 m0;
    /* 0x30 */ Mtx34 m1;
    /* 0x60 */ void * vertSrc;
    /* 0x64 */ void * weights;
    /* 0x68 */ void * vertDst;
    /* 0x6C */ u16 bone0Index;
    /* 0x6E */ u16 bone1Index;
    /* 0x70 */ u16 count;
    /* 0x72 */ u8 vertSrcOffset;
    /* 0x73 */ u8 pad8;
} SK2List;

typedef struct {
    /* 0x00 */ Mtx34 m;
    /* 0x30 */ void * vertSrc;
    /* 0x34 */ void * vertIndices;
    /* 0x38 */ void * vertDst;
    /* 0x3C */ void * weights;
    /* 0x40 */ u16 boneIndex;
    /* 0x42 */ u16 count;
} SKAccList;

typedef struct sHdr {
    /* 0x00 */ u16 numSk1List;
    /* 0x02 */ u16 numSk2List;
    /* 0x04 */ u16 numSkAccList;
    /* 0x06 */ u8 posNrmShift;
    /* 0x07 */ u8 pad8;
    /* 0x08 */ SK1List * sk1ListArray;
    /* 0x0C */ SK2List * sk2ListArray;
    /* 0x10 */ SKAccList * skAccListArray;
    /* 0x14 */ void * skBzeroBase;
    /* 0x18 */ u32 skBzeroSize;
    /* 0x1C */ void * flushIndices;
    /* 0x20 */ u32 flushCount;
} sHdr;

#define GQR_SCALE_MAX 32

// GQRSetup.c
void GQRSetup0(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup1(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup2(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup3(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup4(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup5(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup6(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
void GQRSetup7(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);

// SKNControl.c
extern sAccBuffers SKAccBuffers[2];
extern void * SKBuffers[4];

void SKNIt(sBone **bones, sHdr *hdr);
void SKNInit(void);
void SKNBzero32B(void * base, u32 size);
void SKNLCBzero(void * base, u32 size);

// SKNStats.c
extern sStats SKNStatistics[6];
extern char *StatNames[6];

void SKNPrintStats(void);

// unsorted externs
extern void GQRSetup7(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
extern void GQRSetup6(u32 loadScale, u32 loadType, u32 storeScale, u32 storeType);
extern void SKN1Vecs16Norms16(Mtx34 m, s16 * srcBase, s16 * dstBase, u32 count);
extern void SKN2Vecs16Norms16(Mtx34 m0, Mtx34 m1, u8 * wtBase, s16 * srcBase, s16 * dstBase, u32 count);
extern void SKN2Vecs16Norms16NoTouch(Mtx34 m0, Mtx34 m1, u8 * wtBase, s16 * srcBase, s16 * dstBase, u32 count);
extern void SKNAccVecs16Norms16Iu16(Mtx34 m, u16 count, s16 * srcBase, s16 * dstBase, u16 * indices, u8 * weights);

static inline void SKNLCAccTouchData(u8 buffer, s16 *src, u16 *indices, u8 *weights, u32 count) {
    u32 size = ((count * 3 * 2 * 2) + 0x1F) >> 5;

    LCLoadBlocks(SKAccBuffers[buffer].src, src, size);
    size = ((count * 2) + 0x1F) >> 5;
    LCLoadBlocks(SKAccBuffers[buffer].indices, indices, size);
    LCLoadBlocks(SKAccBuffers[buffer].weights, weights, (count + 31) >> 5);
}

static inline void SKNLC2Vecs16Norms16(Mtx34 m0, Mtx34 m1, u8 buffer, s16 *destination, u32 count, u8 offset, u32 size) {
    SKN2Vecs16Norms16NoTouch(m0, m1, SKBuffers[buffer+1], (void*)((u32)SKBuffers[buffer] + offset), (void*)((u32)SKBuffers[buffer] + offset), count);
    LCStoreBlocks(destination, SKBuffers[buffer], size);
}

static inline u32 SKNLC2TouchData(u8 buffer, void *data, u32 size) {
    size = (size + 0x1F >> 5);
    LCLoadBlocks(SKBuffers[buffer], data, size);
    return size;
}

static inline void SKNLC1Vecs16Norms16(Mtx34 m, s16 *destination, u32 count, u8 buffer, u8 offset, u32 size) {
    SKN1Vecs16Norms16(m, (void*)((u32)SKBuffers[buffer] + offset), (void*)((u32)SKBuffers[buffer] + offset), count);
    LCStoreData(destination, SKBuffers[buffer], size);
}

static inline u32 SKNLCTouchData(u8 buffer, void *data, u32 size) {
    LCLoadData(SKBuffers[buffer], data, size);
    return size;
}

#endif // _DOLPHIN_CP_SKINNING_H_
