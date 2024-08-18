#include "C3/charPipeline.h"
#include "stl/math.h"

static f32 cosf(f32 v) { return cos(v); }
static f32 sinf(f32 v) { return sin(v); }

void CTRLSetScale(Control *control, f32 x, f32 y, f32 z) {
    control->type |= CTRL_SCALE;
    control->type &= ~(CTRL_MTX);
    control->controlParams.srt.s.x = x;
    control->controlParams.srt.s.y = y;
    control->controlParams.srt.s.z = z;
}

void CTRLSetRotation(Control *control, f32 x, f32 y, f32 z) {
    control->type |= CTRL_ROT_EULER;
    control->type &= ~(CTRL_ROT_QUAT);
    control->type &= ~(CTRL_MTX);
    control->controlParams.srt.r.x = x;
    control->controlParams.srt.r.y = y;
    control->controlParams.srt.r.z = z;
}

void CTRLSetQuat(Control *control, f32 x, f32 y, f32 z, f32 w) {
    control->type |= CTRL_ROT_QUAT;
    control->type &= ~(CTRL_ROT_EULER);
    control->type &= ~(CTRL_MTX);
    control->controlParams.srt.r.x = x;
    control->controlParams.srt.r.y = y;
    control->controlParams.srt.r.z = z;
    control->controlParams.srt.r.w = w;
}

void CTRLSetTranslation(Control *control, f32 x, f32 y, f32 z) {
    control->type |= CTRL_TRANS;
    control->type &= ~(CTRL_MTX);
    control->controlParams.srt.t.x = x;
    control->controlParams.srt.t.y = y;
    control->controlParams.srt.t.z = z;
}

void CTRLSetMatrix(Control *control, MtxPtr m) {
    control->type = CTRL_MTX;
    PSMTXCopy(m, control->controlParams.m);
}

void CTRLGetScale(Control *control, f32 *x, f32 *y, f32 *z) {
    *x = control->controlParams.srt.s.x;
    *y = control->controlParams.srt.s.y;
    *z = control->controlParams.srt.s.z;
}

void CTRLGetRotation(Control *control, f32 *x, f32 *y, f32 *z) {
    *x = control->controlParams.srt.r.x;
    *y = control->controlParams.srt.r.y;
    *z = control->controlParams.srt.r.z;
}

void CTRLGetQuat(Control *control, f32 *x, f32 *y, f32 *z, f32 *w) {
    *x = control->controlParams.srt.r.x;
    *y = control->controlParams.srt.r.y;
    *z = control->controlParams.srt.r.z;
    *w = control->controlParams.srt.r.w;
}

void CTRLGetTranslation(Control *control, f32 *x, f32 *y, f32 *z) {
    *x = control->controlParams.srt.t.x;
    *y = control->controlParams.srt.t.y;
    *z = control->controlParams.srt.t.z;
}

void CTRLBuildMatrix(Control *control, Mtx m) {
    f32 nRad;
    f32 nSin;
    f32 nCos;
    f32 temp;

    Mtx a, b, c, d, e;

    if (control->type & CTRL_MTX) {
        PSMTXCopy(control->controlParams.m, m);
        return;
    }
    PSMTXIdentity(m);
    PSMTXIdentity(a);
    PSMTXIdentity(b);
    PSMTXIdentity(c);
    PSMTXIdentity(d);
    PSMTXIdentity(e);
    if (control->type & CTRL_ROT_QUAT) {
        PSMTXQuat(a, &control->controlParams.srt.r);
    } else if (control->type & CTRL_ROT_EULER) {
        if (0.0f != control->controlParams.srt.r.x) {
            nRad = 0.0174532925f * control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;
            a[0][3] = 0.0f;
            a[1][0] = 0.0f;
            a[1][1] = nCos;
            a[1][2] = -nSin;
            a[1][3] = 0.0f;
            a[2][0] = 0.0f;
            a[2][1] = nSin;
            a[2][2] = nCos;
            a[2][3] = 0.0f;
        }
        if (0.0f != control->controlParams.srt.r.y) {
            nRad = 0.0174532925f * control->controlParams.srt.r.y;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = nCos;
            a[0][1] = a[2][1] * nSin;
            a[0][2] = a[2][2] * nSin;
            a[2][0] = -nSin;
            a[2][1] = a[2][1] * nCos;
            a[2][2] = a[2][2] * nCos;
        }
        if (0.0f != control->controlParams.srt.r.z) {
            nRad = 0.0174532925f * control->controlParams.srt.r.z;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[1][0] = a[0][0] * nSin;
            a[0][0] = a[0][0] * nCos;
            temp = a[1][1];
            a[1][1] = (a[0][1] * nSin) + (a[1][1] * nCos);
            a[0][1] = (a[0][1] * nCos) - (temp * nSin);
            temp = m[1][2];
            a[1][2] = (a[0][2] * nSin) + (a[1][2] * nCos);
            a[0][2] = (a[0][2] * nCos) - (temp * nSin);
        }
    }
    
    if (control->type & CTRL_SCALE) {
        b[0][0] = control->controlParams.srt.s.x;
        b[1][1] = control->controlParams.srt.s.y;
        b[2][2] = control->controlParams.srt.s.z;
        if (control->type & CTRL_SCALE_QUAT) {
            PSMTXQuat(c, (Quaternion*)&control->controlParams.q);
            PSMTXInverse(c,d);
        }
    }
    if (control->type & CTRL_TRANS) {
        a[0][3] = control->controlParams.srt.t.x;
        a[1][3] = control->controlParams.srt.t.y;
        a[2][3] = control->controlParams.srt.t.z;
    }

    PSMTXConcat(d,e,e);
    PSMTXConcat(b,e,e);
    PSMTXConcat(c,e,e);
    PSMTXConcat(a,e,e);
    PSMTXCopy(e, m);
}

static Mtx flipY = {{1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}};

static Mtx flipX = {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}};

void CTRLBuildMatrix_FlipY(Control *control, Mtx m) {
    f32 nRad;
    f32 nSin;
    f32 nCos;
    f32 temp;

    Mtx a, b, c, d, e;

    if (control->type & CTRL_MTX) {
        PSMTXCopy(control->controlParams.m, m);
        return;
    }
    PSMTXIdentity(m);
    PSMTXIdentity(a);
    PSMTXIdentity(b);
    PSMTXIdentity(c);
    PSMTXIdentity(d);
    PSMTXIdentity(e);
    if (control->type & CTRL_ROT_QUAT) {
        PSMTXQuat(a, &control->controlParams.srt.r);
    } else if (control->type & CTRL_ROT_EULER) {
        if (0.0f != control->controlParams.srt.r.x) {
            nRad = 0.0174532925f * control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;
            a[0][3] = 0.0f;
            a[1][0] = 0.0f;
            a[1][1] = nCos;
            a[1][2] = -nSin;
            a[1][3] = 0.0f;
            a[2][0] = 0.0f;
            a[2][1] = nSin;
            a[2][2] = nCos;
            a[2][3] = 0.0f;
        }
        if (0.0f != control->controlParams.srt.r.y) {
            nRad = 0.0174532925f * control->controlParams.srt.r.y;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = nCos;
            a[0][1] = a[2][1] * nSin;
            a[0][2] = a[2][2] * nSin;
            a[2][0] = -nSin;
            a[2][1] = a[2][1] * nCos;
            a[2][2] = a[2][2] * nCos;
        }
        if (0.0f != control->controlParams.srt.r.z) {
            nRad = 0.0174532925f * control->controlParams.srt.r.z;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[1][0] = a[0][0] * nSin;
            a[0][0] = a[0][0] * nCos;
            temp = a[1][1];
            a[1][1] = (a[0][1] * nSin) + (a[1][1] * nCos);
            a[0][1] = (a[0][1] * nCos) - (temp * nSin);
            temp = m[1][2];
            a[1][2] = (a[0][2] * nSin) + (a[1][2] * nCos);
            a[0][2] = (a[0][2] * nCos) - (temp * nSin);
        }
    }
    
    if (control->type & CTRL_SCALE) {
        b[0][0] = control->controlParams.srt.s.x;
        b[1][1] = control->controlParams.srt.s.y;
        b[2][2] = control->controlParams.srt.s.z;
        if (control->type & CTRL_SCALE_QUAT) {
            PSMTXQuat(c, (Quaternion*)&control->controlParams.q);
            PSMTXInverse(c,d);
        }
    }
    if (control->type & CTRL_TRANS) {
        a[0][3] = control->controlParams.srt.t.x;
        a[1][3] = control->controlParams.srt.t.y;
        a[2][3] = control->controlParams.srt.t.z;
    }

    PSMTXConcat(d,e,e);
    PSMTXConcat(b,e,e);
    PSMTXConcat(c,e,e);
    PSMTXConcat(a,e,e);

    PSMTXConcat(e, flipY, e);
    PSMTXConcat(flipY, e, e);

    PSMTXCopy(e, m);
}

void CTRLBuildMatrix_FlipX(Control *control, Mtx m) {
    f32 nRad;
    f32 nSin;
    f32 nCos;
    f32 temp;

    Mtx a, b, c, d, e;

    if (control->type & CTRL_MTX) {
        PSMTXCopy(control->controlParams.m, m);
        return;
    }
    PSMTXIdentity(m);
    PSMTXIdentity(a);
    PSMTXIdentity(b);
    PSMTXIdentity(c);
    PSMTXIdentity(d);
    PSMTXIdentity(e);
    if (control->type & CTRL_ROT_QUAT) {
        PSMTXQuat(a, &control->controlParams.srt.r);
    } else if (control->type & CTRL_ROT_EULER) {
        if (0.0f != control->controlParams.srt.r.x) {
            nRad = 0.0174532925f * control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;
            a[0][3] = 0.0f;
            a[1][0] = 0.0f;
            a[1][1] = nCos;
            a[1][2] = -nSin;
            a[1][3] = 0.0f;
            a[2][0] = 0.0f;
            a[2][1] = nSin;
            a[2][2] = nCos;
            a[2][3] = 0.0f;
        }
        if (0.0f != control->controlParams.srt.r.y) {
            nRad = 0.0174532925f * control->controlParams.srt.r.y;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = nCos;
            a[0][1] = a[2][1] * nSin;
            a[0][2] = a[2][2] * nSin;
            a[2][0] = -nSin;
            a[2][1] = a[2][1] * nCos;
            a[2][2] = a[2][2] * nCos;
        }
        if (0.0f != control->controlParams.srt.r.z) {
            nRad = 0.0174532925f * control->controlParams.srt.r.z;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[1][0] = a[0][0] * nSin;
            a[0][0] = a[0][0] * nCos;
            temp = a[1][1];
            a[1][1] = (a[0][1] * nSin) + (a[1][1] * nCos);
            a[0][1] = (a[0][1] * nCos) - (temp * nSin);
            temp = m[1][2];
            a[1][2] = (a[0][2] * nSin) + (a[1][2] * nCos);
            a[0][2] = (a[0][2] * nCos) - (temp * nSin);
        }
    }
    
    if (control->type & CTRL_SCALE) {
        b[0][0] = control->controlParams.srt.s.x;
        b[1][1] = control->controlParams.srt.s.y;
        b[2][2] = control->controlParams.srt.s.z;
        if (control->type & CTRL_SCALE_QUAT) {
            PSMTXQuat(c, (Quaternion*)&control->controlParams.q);
            PSMTXInverse(c,d);
        }
    }
    if (control->type & CTRL_TRANS) {
        a[0][3] = control->controlParams.srt.t.x;
        a[1][3] = control->controlParams.srt.t.y;
        a[2][3] = control->controlParams.srt.t.z;
    }

    PSMTXConcat(d,e,e);
    PSMTXConcat(b,e,e);
    PSMTXConcat(c,e,e);
    PSMTXConcat(a,e,e);

    PSMTXConcat(e, flipY, e);
    PSMTXConcat(flipY, e, e);

    PSMTXCopy(e, m);
}

void CTRLBuildMatrix_Flip(Control *control, Mtx m, u32 r5) {
    f32 nRad;
    f32 nSin;
    f32 nCos;
    f32 temp;

    Mtx a, b, c, d, e, f;

    if (control->type & CTRL_MTX) {
        PSMTXCopy(control->controlParams.m, m);
        return;
    }

    PSMTX44Identity(f);

    if (r5 & 1)
    {
        f[0][0] = -1.0;
    }
    if (r5 & 2)
    {
        f[1][1] = -1.0;
    }
    if (r5 & 4)
    {
        f[2][2] = -1.0;
    }

    PSMTXIdentity(m);
    PSMTXIdentity(a);
    PSMTXIdentity(b);
    PSMTXIdentity(c);
    PSMTXIdentity(d);
    PSMTXIdentity(e);
    if (control->type & CTRL_ROT_QUAT) {
        PSMTXQuat(a, &control->controlParams.srt.r);
    } else if (control->type & CTRL_ROT_EULER) {
        if (0.0f != control->controlParams.srt.r.x) {
            nRad = 0.0174532925f * control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;
            a[0][3] = 0.0f;
            a[1][0] = 0.0f;
            a[1][1] = nCos;
            a[1][2] = -nSin;
            a[1][3] = 0.0f;
            a[2][0] = 0.0f;
            a[2][1] = nSin;
            a[2][2] = nCos;
            a[2][3] = 0.0f;
        }
        if (0.0f != control->controlParams.srt.r.y) {
            nRad = 0.0174532925f * control->controlParams.srt.r.y;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[0][0] = nCos;
            a[0][1] = a[2][1] * nSin;
            a[0][2] = a[2][2] * nSin;
            a[2][0] = -nSin;
            a[2][1] = a[2][1] * nCos;
            a[2][2] = a[2][2] * nCos;
        }
        if (0.0f != control->controlParams.srt.r.z) {
            nRad = 0.0174532925f * control->controlParams.srt.r.z;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            a[1][0] = a[0][0] * nSin;
            a[0][0] = a[0][0] * nCos;
            temp = a[1][1];
            a[1][1] = (a[0][1] * nSin) + (a[1][1] * nCos);
            a[0][1] = (a[0][1] * nCos) - (temp * nSin);
            temp = m[1][2];
            a[1][2] = (a[0][2] * nSin) + (a[1][2] * nCos);
            a[0][2] = (a[0][2] * nCos) - (temp * nSin);
        }
    }
    
    if (control->type & CTRL_SCALE) {
        b[0][0] = control->controlParams.srt.s.x;
        b[1][1] = control->controlParams.srt.s.y;
        b[2][2] = control->controlParams.srt.s.z;
        if (control->type & CTRL_SCALE_QUAT) {
            PSMTXQuat(c, (Quaternion*)&control->controlParams.q);
            PSMTXInverse(c,d);
        }
    }
    if (control->type & CTRL_TRANS) {
        a[0][3] = control->controlParams.srt.t.x;
        a[1][3] = control->controlParams.srt.t.y;
        a[2][3] = control->controlParams.srt.t.z;
    }

    PSMTXConcat(d,e,e);
    PSMTXConcat(b,e,e);
    PSMTXConcat(c,e,e);
    PSMTXConcat(a,e,e);

    PSMTXConcat(e, f, e);
    PSMTXConcat(f, e, e);

    PSMTXCopy(e, m);
}

void CTRLBuildInverseMatrix(Control *control, Mtx m) {
    f32 nRad;
    f32 nSin;
    f32 nCos;
    f32 temp;
    Quaternion q;

    if (control->type & CTRL_MTX) {
        PSMTXInverse(control->controlParams.m, m);
        return;
    }
    if (control->type & CTRL_ROT_QUAT) {
        q.x = -control->controlParams.srt.r.x;
        q.y = -control->controlParams.srt.r.y;
        q.z = -control->controlParams.srt.r.z;
        q.w = -control->controlParams.srt.r.w;
        MTXQuat(m, &q);
    } else if (control->type & CTRL_ROT_EULER) {
        if (0.0f != control->controlParams.srt.r.z) {
            nRad = 0.0174532925f * -control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            m[0][0] = nCos;
            m[0][1] = -nSin;
            m[0][2] = 0.0f;
            m[0][3] = 0.0f;
            m[1][0] = nSin;
            m[1][1] = nCos;
            m[1][2] = 0.0f;
            m[1][3] = 0.0f;
            m[2][0] = 0.0f;
            m[2][1] = 0.0f;
            m[2][2] = 1.0f;
            m[2][3] = 0.0f;
        } else {
            PSMTXIdentity(m);
        }
        if (0.0f != control->controlParams.srt.r.y) {
            nRad = 0.0174532925f * -control->controlParams.srt.r.y;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            m[2][0] = m[0][0] * -nSin;
            m[2][1] = m[0][1] * -nSin;
            m[2][2] = nCos;
            m[0][0] = m[0][0] * nCos;
            m[0][1] = m[0][1] * nCos;
            m[0][2] = nSin;
        }
        if (0.0f != control->controlParams.srt.r.x) {
            nRad = 0.0174532925f * -control->controlParams.srt.r.x;
            nSin = sinf(nRad);
            nCos = cosf(nRad);
            m[1][2] = m[2][2] * -nSin;
            m[2][2] = m[2][2] * nCos;
            temp = m[1][0];
            m[1][0] = (m[1][0] * nCos) - (m[2][0] * nSin);
            m[2][0] = (m[2][0] * nCos) + (temp * nSin);
            temp = m[1][1];
            m[1][1] = (m[1][1] * nCos) - (m[2][1] * nSin);
            m[2][1] = (m[2][1] * nCos) + (temp * nSin);
        }
    } else {
        PSMTXIdentity(m);
    }
    if (control->type & CTRL_TRANS) {
        if ((control->type & CTRL_ROT_QUAT) || (control->type & CTRL_ROT_EULER)) {
            m[0][3] = -((m[0][2] * control->controlParams.srt.t.z) + ((m[0][0] * control->controlParams.srt.t.x) + (m[0][1] * control->controlParams.srt.t.y)));
            m[1][3] = -((m[1][2] * control->controlParams.srt.t.z) + ((m[1][0] * control->controlParams.srt.t.x) + (m[1][1] * control->controlParams.srt.t.y)));
            m[2][3] = -((m[2][2] * control->controlParams.srt.t.z) + ((m[2][0] * control->controlParams.srt.t.x) + (m[2][1] * control->controlParams.srt.t.y)));
        } else {
            m[0][3] = -control->controlParams.srt.t.x;
            m[1][3] = -control->controlParams.srt.t.y;
            m[2][3] = -control->controlParams.srt.t.z;
        }
    }
    if (control->type & CTRL_SCALE) {
        q.x = 1.0f / control->controlParams.srt.s.x;
        q.y = 1.0f / control->controlParams.srt.s.y;
        q.z = 1.0f / control->controlParams.srt.s.z;
        m[0][0] *= q.x;
        m[0][1] *= q.x;
        m[0][2] *= q.x;
        m[0][3] *= q.x;
        m[1][0] *= q.y;
        m[1][1] *= q.y;
        m[1][2] *= q.y;
        m[1][3] *= q.y;
        m[2][0] *= q.z;
        m[2][1] *= q.z;
        m[2][2] *= q.z;
        m[2][3] *= q.z;
    }
}

void fn_800B84AC(Control *control, Control *r3, Control *r4, float f1)
{
    Quaternion q1 = r3->controlParams.q;
    if (f1 >= 0.0)
    {
        control->controlParams.q = q1;
    }

    PSQUATDotProduct(0,0);
    C_QUATSlerp(0, 0, 0, 0);
}

void CTRL_SetQuatUnk(Control *control, float x, float y, float z, float w)
{
    control->type |= CTRL_SCALE_QUAT;
    control->type &= ~CTRL_MTX;
    control->controlParams.q.x = x;
    control->controlParams.q.y = y;
    control->controlParams.q.z = z;
    control->controlParams.q.w = w;
}

void CTRL_GetQuatUnk(const Control *control, float *x, float *y, float *z, float *w)
{
    *x = control->controlParams.q.x;
    *y = control->controlParams.q.y;
    *z = control->controlParams.q.z;
    *w = control->controlParams.q.w;
}

void fn_800B81C0()
{

}

void fn_800B806C()
{
    
}

void fn_800B7D3C()
{
    
}