#include "Dolphin/mtxext.h"

#ifdef __MWERKS__ // clang-format off
asm void PSMTX44MultVec(const register Mtx44 mtx, const register Vec *src,register Vec *dst)
{
    psq_l       f0, 0x0(src), 0, 0
    psq_l       f2, 0x30(mtx), 0, 0
    psq_l       f1, 0x8(src), 1, 0
    ps_mul      f4, f0, f2
    psq_l       f3, 0x38(mtx), 0, 0
    ps_madd     f5, f1, f3, f4
    ps_merge11  f12, f1, f1
    ps_sum0     f13, f5, f5, f5
    psq_l       f4, 0x0(mtx), 0, 0
    ps_merge00  f13, f13, f13
    psq_l       f5, 0x8(mtx), 0, 0
    ps_div      f13, f12, f13
    psq_l       f6, 0x10(mtx), 0, 0
    psq_l       f7, 0x18(mtx), 0, 0
    psq_l       f8, 0x20(mtx), 0, 0
    psq_l       f9, 0x28(mtx), 0, 0
    ps_mul      f4, f0, f4
    ps_madd     f2, f1, f5, f4
    ps_mul      f6, f0, f6
    ps_madd     f3, f1, f7, f6
    ps_mul      f8, f0, f8
    ps_sum0     f2, f2, f2, f2
    ps_madd     f9, f1, f9, f8
    ps_sum1     f2, f3, f2, f3
    ps_sum0     f3, f9, f9, f9
    ps_mul      f2, f2, f13
    psq_st      f2, 0x0(dst), 0, 0
    ps_mul      f3, f3, f13
    psq_st      f3, 0x8(dst), 1, 0
}
#endif // clang-format on

#ifdef __MWERKS__ // clang-format off
asm void PSMTX44MultVecArray(register const Mtx44 mtx, register const Vec *src, register Vec *dst, register u32 count)
{
    nofralloc
    stwu r1, -0x10(r1)
    subi count, count, 0x1
    psq_l f6, 0x30(mtx), 0, 0
    mtctr count
    psq_l f8, 0x0(src), 0, 0
    subi dst, dst, 0x4
    psq_l f7, 0x38(mtx), 0, 0
    psq_lu f9, 0x8(src), 1, 0
    ps_mul f13, f6, f8
    psq_l f0, 0x0(mtx), 0, 0
    stfd f14, 0x8(r1)
    ps_madd f13, f7, f9, f13
    psq_l f2, 0x10(mtx), 0, 0
    ps_merge11 f14, f9, f9
    ps_mul f10, f0, f8
    psq_l f4, 0x20(mtx), 0, 0
    ps_mul f11, f2, f8
    psq_l f1, 0x8(mtx), 0, 0
    ps_mul f12, f4, f8
    psq_l f3, 0x18(mtx), 0, 0
    ps_sum0 f13, f13, f13, f13
    psq_l f5, 0x28(mtx), 0, 0
L_80076258:
    ps_madd f10, f1, f9, f10
    ps_madd f11, f3, f9, f11
    ps_madd f12, f5, f9, f12
    ps_sum0 f10, f10, f10, f10
    ps_sum0 f11, f11, f11, f11
    ps_sum0 f12, f12, f12, f12
    ps_div f13, f14, f13
    psq_lu f8, 0x4(src), 0, 0
    psq_lu f9, 0x8(src), 1, 0
    ps_mul f10, f10, f13
    psq_stu f10, 0x4(dst), 1, 0
    ps_mul f11, f11, f13
    psq_stu f11, 0x4(dst), 1, 0
    ps_mul f12, f12, f13
    psq_stu f12, 0x4(dst), 1, 0
    ps_mul f13, f6, f8
    ps_mul f10, f0, f8
    ps_mul f11, f2, f8
    ps_madd f13, f7, f9, f13
    ps_mul f12, f4, f8
    ps_sum0 f13, f13, f13, f13
    bdnz L_80076258
    ps_madd f10, f1, f9, f10
    ps_madd f11, f3, f9, f11
    ps_madd f12, f5, f9, f12
    ps_sum0 f10, f10, f10, f10
    ps_sum0 f11, f11, f11, f11
    ps_sum0 f12, f12, f12, f12
    ps_div f13, f14, f13
    ps_mul f10, f10, f13
    psq_st f10, 0x4(dst), 1, 0
    ps_mul f11, f11, f13
    psq_st f11, 0x8(dst), 1, 0
    ps_mul f12, f12, f13
    psq_st f12, 0xc(dst), 1, 0
    lfd f14, 0x8(r1)
    addi r1, r1, 0x10
    blr
}
#endif // clang-format on
