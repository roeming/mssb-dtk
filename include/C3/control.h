#ifndef _DOLPHIN_CP_CONTROL_H_
#define _DOLPHIN_CP_CONTROL_H_

#include "Dolphin/mtx.h"

// bitflags
#define CTRL_SCALE       (1 << 0) // 0x1
#define CTRL_ROT_EULER   (1 << 1) // 0x2
#define CTRL_ROT_QUAT    (1 << 2) // 0x4
#define CTRL_TRANS       (1 << 3) // 0x8
#define CTRL_MTX         (1 << 4) // 0x10
#define CTRL_SCALE_QUAT  (1 << 5) // 0x20

typedef struct Srt {
    /* 0x00 */ Vec s;
    /* 0x0C */ Quaternion r;
    /* 0x1C */ Vec t;
} Srt;

typedef union ControlParams {
    struct{
    /* 0x00 */ Srt srt;
        Quaternion q;
    };
    /* 0x00 */ struct {
        /* 0x00 */ Mtx m;
    };
} ControlParams;

typedef struct Control {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad8;
    /* 0x02 */ u16 pad16;
    /* 0x04 */ ControlParams controlParams;
} Control;

// control.c
void CTRLSetScale(Control *control, f32 x, f32 y, f32 z);
void CTRLSetRotation(Control *control, f32 x, f32 y, f32 z);
void CTRLSetQuat(Control *control, f32 x, f32 y, f32 z, f32 w);
void CTRLSetTranslation(Control *control, f32 x, f32 y, f32 z);
void CTRLSetMatrix(Control *control, MtxPtr m);
void CTRLGetScale(Control *control, f32 *x, f32 *y, f32 *z);
void CTRLGetRotation(Control *control, f32 *x, f32 *y, f32 *z);
void CTRLGetQuat(Control *control, f32 *x, f32 *y, f32 *z, f32 *w);
void CTRLGetTranslation(Control *control, f32 *x, f32 *y, f32 *z);
void CTRLBuildMatrix(Control *control, Mtx m);
void CTRLBuildInverseMatrix(Control *control, Mtx m);

#endif // _DOLPHIN_CP_CONTROL_H_
