#include "Dolphin/mtx.h"

#ifdef __MWERKS__ // clang-format off
asm void PSMTXReorder(register const Mtx34 src, register Mtx43 dst)
{
    psq_l f0, 0x0(src), 0, 0
    psq_l f2, 0x10(src), 0, 0
    psq_l f4, 0x20(src), 0, 0
    psq_l f1, 0x8(src), 0, 0
    ps_merge00 f6, f0, f2
    psq_l f3, 0x18(src), 0, 0
    ps_merge01 f12, f4, f0
    psq_l f5, 0x28(src), 0, 0
    ps_merge11 f7, f2, f4
    psq_st f6, 0x0(dst), 0, 0
    ps_merge00 f8, f1, f3
    psq_st f12, 0x8(dst), 0, 0
    ps_merge01 f9, f5, f1
    psq_st f7, 0x10(dst), 0, 0
    ps_merge11 f10, f3, f5
    psq_st f8, 0x18(dst), 0, 0
    psq_st f9, 0x20(dst), 0, 0
    psq_st f10, 0x28(dst), 0, 0
}
#endif // clang-format on