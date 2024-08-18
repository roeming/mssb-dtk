#ifndef _DOLPHIN_CP_LIGHT_H_
#define _DOLPHIN_CP_LIGHT_H_

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

enum PARENT_TYPE {
    PARENT_BONE = 0,
    PARENT_DISP_OBJ = 1,
    PARENT_MTX = 2,
};

typedef struct Light {
    /* 0x00 */ GXLightObj lt_obj;
    /* 0x40 */ Vec position;
    /* 0x4C */ Vec worldPosition;
    /* 0x58 */ Vec direction;
    /* 0x64 */ Vec worldDirection;
    /* 0x70 */ GXColor color;
    /* 0x74 */ Control control;
    /* 0xA8 */ MtxPtr parent;
    /* 0xAC */ struct ANIMPipe *animPipe;
} Light;

#endif // _DOLPHIN_CP_LIGHT_H_
