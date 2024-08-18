#ifndef _DOLPHIN_MTXEXT_H
#define _DOLPHIN_MTXEXT_H

#include "Dolphin/mtx.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

void PSMTX44MultVec(const Mtx44 mtx, const Vec *src, Vec *dst);
void PSMTX44MultVecArray(const Mtx44 mtx, const Vec *src, Vec *dst, u32 count);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef _DOLPHIN_MTXEXT_H