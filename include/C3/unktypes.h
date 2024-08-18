#ifndef _DOLPHIN_CP_UNKTYPES_H_
#define _DOLPHIN_CP_UNKTYPES_H_

// uncategorized types go in here.

#include "Dolphin/gx.h"

#include "charPipeline/texPalette.h"

typedef struct {
    /* 0x00 */ char *positionArray;
    /* 0x04 */ u16 numPositions;
    /* 0x06 */ u8 quantizeInfo;
    /* 0x07 */ u8 compCount;
} PositionData;

typedef struct {
    /* 0x00 */ char * colorArray;
    /* 0x04 */ u16 numColors;
    /* 0x06 */ u8 quantizeInfo;
    /* 0x07 */ u8 compCount;
} ColorData;

typedef struct {
    /* 0x00 */ u32 versionNumber;
    /* 0x04 */ u32 userDataSize;
    /* 0x08 */ void *userData;
    /* 0x0C */ u32 numDescriptors;
    /* 0x10 */ TEXDescriptorPtr descriptorArray;
} TEXPaletteData, *TEXPaletteDataPtr;

typedef struct {
    /* 0x00 */ char * textureCoordArray;
    /* 0x04 */ u16 numTextureCoords;
    /* 0x06 */ u8 quantizeInfo;
    /* 0x07 */ u8 compCount;
    /* 0x08 */ char * texturePaletteName;
    /* 0x0C */ TEXPalettePtr texturePalette;
} TextureData;

typedef struct {
    /* 0x00 */ char * normalArray;
    /* 0x04 */ u16 numNormals;
    /* 0x06 */ u8 quantizeInfo;
    /* 0x07 */ u8 compCount;
    /* 0x08 */ f32 ambientPercentage;
} LightingData;

typedef struct {
    /* 0x00 */ u8 id;
    /* 0x01 */ u8 pad8;
    /* 0x02 */ u16 pad16;
    /* 0x04 */ u32 setting;
    /* 0x08 */ char * primitiveList;
    /* 0x0C */ u32 listSize;
} DisplayStateList;

typedef struct {
    /* 0x00 */ char * primitiveBank;
    /* 0x04 */ DisplayStateList * displayStateList;
    /* 0x08 */ u16 numStateEntries;
    /* 0x0A */ u16 pad16;
} DisplayData;

typedef struct KeyFrame {
    /* 0x00 */ f32 time;
    /* 0x04 */ char * setting;
    /* 0x08 */ char * interpolation;
} KeyFrame;

typedef struct {
    /* 0x00 */ char * Prev;
    /* 0x04 */ char * Next;
    /* 0x08 */ char * Parent;
    /* 0x0C */ char * Children;
} Branch;

#endif // _DOLPHIN_CP_UNKTYPES_H_
