#include "Dolphin/mtx.h"
#include "math.h"

void C_QUATAdd(void)
{
    // UNUSED
    // I hate this, but I don't know another way to get the sdata2 to match
    register f32 sdataReordering;
    register f64 sdataReordering64;
    sdataReordering = 0.00001f;
    sdataReordering = 0.f;
    sdataReordering64 = 0.5;
    sdataReordering64 = 3.;
    sdataReordering = 1.f;
#ifdef __MWERKS__ // clang-format off
    asm {
        nop
    }
#endif // clang-format on
}

void PSQUATAdd(register const Quaternion *a, register const Quaternion *b, register Quaternion *sum)
{
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l   f0, 0x0(a), 0, 0
        psq_l   f1, 0x0(b), 0, 0
        ps_add  f0, f0, f1
        psq_st  f0, 0x0(sum), 0, 0
        psq_l   f0, 0x8(a), 0, 0
        psq_l   f1, 0x8(b), 0, 0
        ps_add  f0, f0, f1
        psq_st  f0, 0x8(sum), 0, 0
    }
#endif // clang-format on
}

void C_QUATSubtract(void)
{
    // UNUSED
}

void PSQUATSubtract(void)
{
    // UNUSED
}

void C_QUATMultiply(void)
{
    // UNUSED
}

void PSQUATMultiply(register const Quaternion *a, register const Quaternion *b, register Quaternion *product)
{
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l f0, 0x0(a), 0, 0
        psq_l f1, 0x8(a), 0, 0
        psq_l f2, 0x0(b), 0, 0
        ps_neg f5, f0
        psq_l f3, 0x8(b), 0, 0
        ps_neg f6, f1
        ps_merge01 f4, f5, f0
        ps_muls0 f7, f1, f2
        ps_muls0 f5, f5, f2
        ps_merge01 f1, f6, f1
        ps_muls1 f8, f4, f2
        ps_madds0 f7, f4, f3, f7
        ps_muls1 f2, f1, f2
        ps_madds0 f5, f1, f3, f5
        ps_madds1 f8, f6, f3, f8
        ps_merge10 f7, f7, f7
        ps_madds1 f2, f0, f3, f2
        ps_merge10 f5, f5, f5
        ps_add f7, f7, f2
        psq_st f7, 0x0(product), 0, 0
        ps_sub f5, f5, f8
        psq_st f5, 0x8(product), 0, 0
    }
#endif // clang-format on
}

void C_QUATScale(void)
{
    // UNUSED
}

void PSQUATScale(register const Quaternion *a, register const f32 scale, register Quaternion *dst)
{
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l       f0, 0x0(a), 0, 0
        psq_l       f2, 0x8(a), 0, 0
        ps_muls0    f0, f0, scale
        psq_st      f0, 0x0(dst), 0, 0    
        ps_muls0    f2, f2, scale
        psq_st      f2, 0x8(dst), 0, 0    
    }
#endif // clang-format on
}

void C_QUATDotProduct(void)
{
    // UNUSED
}

f32 PSQUATDotProduct(register const Quaternion *a, register const Quaternion *b)
{
    register f32 ret;
#ifdef __MWERKS__ // clang-format off
    asm {
        psq_l   f0, 0x0(a), 0, 0
        psq_l   ret, 0x0(b), 0, 0
        ps_mul  ret, f0, ret
        psq_l   f0, 0x8(a), 0, 0
        psq_l   f2, 0x8(b), 0, 0
        ps_madd ret, f0, f2, ret
        ps_sum0 ret, ret, ret, ret
    }
#endif // clang-format on
    return ret;
}

void C_QUATNormalize(void)
{
    // UNUSED
}

#ifdef __MWERKS__ // clang-format off
void PSQUATNormalize(register const Quaternion *a, register Quaternion *normalized)
{
    register f32 epsilon, half, three;
    epsilon = 0.00001f;
    half = 0.5f;
    three = 3.0f;
    asm {
        psq_l   f0, 0x0(a), 0, 0
        ps_mul  f2, f0, f0
        psq_l   f1, 0x8(a), 0, 0
        ps_sub  f5, epsilon, epsilon
        ps_madd f2, f1, f1, f2
        ps_sum0 f2, f2, f2, f2
        frsqrte f3, f2
        ps_sub  epsilon, f2, epsilon
        fmul    f6, f3, f3
        fmul    f3, f3, half
        fnmsub  f6, f6, f2, three
        fmul    f3, f6, f3
        ps_sel  f3, epsilon, f3, f5
        ps_muls0 f0, f0, f3
        ps_muls0 f1, f1, f3
        psq_st  f0, 0(normalized), 0, 0
        psq_st  f1, 8(normalized), 0, 0
    }
}
#endif // clang-format on

void C_QUATInverse(void)
{
    // UNUSED
}

void PSQUATInverse(void)
{
    // UNUSED
}

void C_QUATDivide(void)
{
    // UNUSED
}

void PSQUATDivide(void)
{
    // UNUSED
}

void C_QUATExp(void)
{
    // UNUSED
}

void C_QUATLogN(void)
{
    // UNUSED
}

void C_QUATMakeClosest(void)
{
    // UNUSED
}

void C_QUATRotAxisRad(Quaternion *q, const Vec *v, f32 radians)
{
    f32 s;
    f32 c;
    f32 half;
    Vec t;

    PSVECNormalize(v, &t);
    half = radians * 0.5f; 
    s = sinf(half);
    c = cosf(half);

    q->x = s * t.x;
    q->y = s * t.y;
    q->z = s * t.z;
    q->w = c;
}

void C_QUATMtx(Quaternion *r, const Mtx m)
{
    f32 tr, s;
    s32 i, j, k;
    s32 nxt[3] = {1, 2, 0};
    f32 q[3];

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f)
    {
        s = (f32)dolsqrtf(tr + 1.0f);
        r->w = s * 0.5f;
        s = 0.5f / s;
        r->x = (m[2][1] - m[1][2]) * s;
        r->y = (m[0][2] - m[2][0]) * s;
        r->z = (m[1][0] - m[0][1]) * s;
    }
    else
    {
        i = 0;
        if (m[1][1] > m[0][0])
        {
            i = 1;
        }

        if (m[2][2] > m[i][i])
        {
            i = 2;
        }

        j = nxt[i];
        k = nxt[j];
        s = (f32)dolsqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5f;

        if (s != 0.0f)
        {
            s = 0.5f / s;
        }

        r->w = (m[k][j] - m[j][k]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        r->x = q[0];
        r->y = q[1];
        r->z = q[2];
    }
}

void C_QUATLerp(void)
{
    // UNUSED
}

void C_QUATSlerp(const Quaternion *p, const Quaternion *q, Quaternion *r, f32 t)
{
    f32 t1, t2, t3, t4, t5;

    t3 = p->x * q->x + p->y * q->y + p->z * q->z + p->w * q->w;
    t5 = 1.0f;

    if (t3 < 0.0f)
    {
        t3 = -t3;
        t5 = -t5;
    }

    if (t3 <= 0.99999f)
    {
        t1 = acosf(t3);
        t2 = sinf(t1);
        t4 = sinf((1.0f - t) * t1) / t2;
        t5 *= sinf(t * t1) / t2;
    }
    else
    {
        t4 = 1.0f - t;
        t5 = t5 * t;
    }

    r->x = t4 * p->x + t5 * q->x;
    r->y = t4 * p->y + t5 * q->y;
    r->z = t4 * p->z + t5 * q->z;
    r->w = t4 * p->w + t5 * q->w;
}

void C_QUATSquad(void)
{
    // UNUSED
}

void C_QUATCompA(void)
{
    // UNUSED
}
