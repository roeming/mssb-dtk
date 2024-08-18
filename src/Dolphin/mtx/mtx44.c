#include "Dolphin/mtx.h"
#include "math.h"

/**
 * @note Address: N/A
 * @note Size: 0x9C
 */
void C_MTXFrustum(Mtx44 m, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6)
{
	f32 tmp = 1.0f / (arg4 - arg3);
	m[0][0] = (2 * arg5) * tmp;
	m[0][1] = 0.0f;
	m[0][2] = (arg4 + arg3) * tmp;
	m[0][3] = 0.0f;
	tmp = 1.0f / (arg1 - arg2);
	m[1][0] = 0.0f;
	m[1][1] = (2 * arg5) * tmp;
	m[1][2] = (arg1 + arg2) * tmp;
	m[1][3] = 0.0f;
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	tmp = 1.0f / (arg6 - arg5);
	m[2][2] = -(arg5)*tmp;
	m[2][3] = -(arg6 * arg5) * tmp;
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = -1.0f;
	m[3][3] = 0.0f;
}

/**
 * @note Address: 0x800EAD08
 * @note Size: 0xD0
 */
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f)
{
	f32 angle = fovY * 0.5f;
	f32 cot;
	f32 tmp;
	angle = MTXDegToRad(angle);
	cot = 1.0f / tanf(angle);
	m[0][0] = cot / aspect;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;
	m[1][0] = 0.0f;
	m[1][1] = cot;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	tmp = 1.0f / (f - n);
	m[2][2] = -(n)*tmp;
	m[2][3] = -(f * n) * tmp;
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = -1.0f;
	m[3][3] = 0.0f;
}

/**
 * @note Address: 0x800EADD8
 * @note Size: 0x98
 */
void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f)
{
	f32 tmp = 1.0f / (r - l);
	m[0][0] = 2.0f * tmp;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = -(r + l) * tmp;
	tmp = 1.0f / (t - b);
	m[1][0] = 0.0f;
	m[1][1] = 2.0f * tmp;
	m[1][2] = 0.0f;
	m[1][3] = -(t + b) * tmp;
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	tmp = 1.0f / (f - n);
	m[2][2] = -(1.0f) * tmp;
	m[2][3] = -(f)*tmp;
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

/**
 * @note Address: N/A
 * @note Size: 0x4C
 */
void C_MTX44Identity(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x34
 */
void PSMTX44Identity(register Mtx44 mtx)
{
	register f32 one = 1.f;
	register f32 zero = 0.f;
#ifdef __MWERKS__ // clang-format off
	asm {
		stfs 	one, 	0x00(mtx)
		psq_st	zero, 	0x04(mtx), 0, 0
		psq_st	zero, 	0x0c(mtx), 0, 0
		stfs 	one, 	0x14(mtx)
		psq_st	zero, 	0x18(mtx), 0, 0
		psq_st	zero, 	0x20(mtx), 0, 0
		stfs 	one, 	0x28(mtx)
		psq_st	zero, 	0x2c(mtx), 0, 0
		psq_st	zero, 	0x34(mtx), 0, 0
		stfs 	one, 	0x3c(mtx)
	}
#endif // clang-format on
}

/**
 * @note Address: N/A
 * @note Size: 0x8C
 */
void C_MTX44Copy(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: 0x800EAE70
 * @note Size: 0x44
 */
#ifdef __MWERKS__ // clang-format off
asm void PSMTX44Copy(const register Mtx44 src, register Mtx44 dest)
{
	psq_l fp1, 0(src), 0, 0;
	psq_st fp1, 0(dest), 0, 0;
	psq_l fp1, 8(src), 0, 0;
	psq_st fp1, 8(dest), 0, 0;
	psq_l fp1, 0x10(src), 0, 0;
	psq_st fp1, 0x10(dest), 0, 0;
	psq_l fp1, 0x18(src), 0, 0;
	psq_st fp1, 0x18(dest), 0, 0;
	psq_l fp1, 0x20(src), 0, 0;
	psq_st fp1, 0x20(dest), 0, 0;
	psq_l fp1, 0x28(src), 0, 0;
	psq_st fp1, 0x28(dest), 0, 0;
	psq_l fp1, 0x30(src), 0, 0;
	psq_st fp1, 0x30(dest), 0, 0;
	psq_l fp1, 0x38(src), 0, 0;
	psq_st fp1, 0x38(dest), 0, 0;
}
#endif // clang-format on

/**
 * @note Address: N/A
 * @note Size: 0x450
 */
void C_MTX44Concat(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x104
 */
#ifdef __MWERKS__ // clang-format off
asm void PSMTX44Concat(const register Mtx44 A, const register Mtx44 B, register Mtx44 concat)
{
	psq_l f0, 0x0(A), 0, 0
	psq_l f2, 0x0(B), 0, 0
	ps_muls0 f6, f2, f0
	psq_l f3, 0x10(B), 0, 0
	psq_l f4, 0x20(B), 0, 0
	ps_madds1 f6, f3, f0, f6
	psq_l f1, 0x8(A), 0, 0
	psq_l f5, 0x30(B), 0, 0
	ps_madds0 f6, f4, f1, f6
	psq_l f0, 0x10(A), 0, 0
	ps_madds1 f6, f5, f1, f6
	psq_l f1, 0x18(A), 0, 0
	ps_muls0 f8, f2, f0
	ps_madds1 f8, f3, f0, f8
	psq_l f0, 0x20(A), 0, 0
	ps_madds0 f8, f4, f1, f8
	ps_madds1 f8, f5, f1, f8
	psq_l f1, 0x28(A), 0, 0
	ps_muls0 f10, f2, f0
	ps_madds1 f10, f3, f0, f10
	psq_l f0, 0x30(A), 0, 0
	ps_madds0 f10, f4, f1, f10
	ps_madds1 f10, f5, f1, f10
	psq_l f1, 0x38(A), 0, 0
	ps_muls0 f12, f2, f0
	psq_l f2, 0x8(B), 0, 0
	ps_madds1 f12, f3, f0, f12
	psq_l f0, 0x0(A), 0, 0
	ps_madds0 f12, f4, f1, f12
	psq_l f3, 0x18(B), 0, 0
	ps_madds1 f12, f5, f1, f12
	psq_l f1, 0x8(A), 0, 0
	ps_muls0 f7, f2, f0
	psq_l f4, 0x28(B), 0, 0
	ps_madds1 f7, f3, f0, f7
	psq_l f5, 0x38(B), 0, 0
	ps_madds0 f7, f4, f1, f7
	psq_l f0, 0x10(A), 0, 0
	ps_madds1 f7, f5, f1, f7
	psq_l f1, 0x18(A), 0, 0
	ps_muls0 f9, f2, f0
	psq_st f6, 0x0(r5), 0, 0
	ps_madds1 f9, f3, f0, f9
	psq_l f0, 0x20(A), 0, 0
	ps_madds0 f9, f4, f1, f9
	psq_st f8, 0x10(r5), 0, 0
	ps_madds1 f9, f5, f1, f9
	psq_l f1, 0x28(A), 0, 0
	ps_muls0 f11, f2, f0
	psq_st f10, 0x20(r5), 0, 0
	ps_madds1 f11, f3, f0, f11
	psq_l f0, 0x30(A), 0, 0
	ps_madds0 f11, f4, f1, f11
	psq_st f12, 0x30(r5), 0, 0
	ps_madds1 f11, f5, f1, f11
	psq_l f1, 0x38(A), 0, 0
	ps_muls0 f13, f2, f0
	psq_st f7, 0x8(r5), 0, 0
	ps_madds1 f13, f3, f0, f13
	psq_st f9, 0x18(r5), 0, 0
	ps_madds0 f13, f4, f1, f13
	psq_st f11, 0x28(r5), 0, 0
	ps_madds1 f13, f5, f1, f13
	psq_st f13, 0x38(r5), 0, 0
}
#endif // clang-format on

/**
 * @note Address: N/A
 * @note Size: 0xC4
 */
void C_MTX44Transpose(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x64
 */
void PSMTX44Transpose(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x3F0
 */
u32 C_MTX44Inverse(const Mtx44 src, Mtx44 dst)
{
	Mtx44 tempM;
	f32 f;
	s32 i, i2, i3;

	PSMTX44Copy(src, tempM);
	PSMTX44Identity(dst);

	for (i = 0; i < 4; ++i)
	{
		f32 max = 0.0f;
		s32 tmp_i = i;

		// ---- partial pivoting -----
		for (i3 = i; i3 < 4; i3++)
		{
			f32 tmp;
			tmp = fabsf(tempM[i3][i]);
			if (tmp > max)
			{
				max = tmp;
				tmp_i = i3;
			}
		}

		if (max == 0.0f)
		{
			return 0;
		}

		if (tmp_i != i)
		{
			for (i3 = 0; i3 < 4; i3++)
			{
				f32 tmp;
				tmp = tempM[i][i3];
				tempM[i][i3] = tempM[tmp_i][i3];
				tempM[tmp_i][i3] = tmp;

				tmp = dst[i][i3];
				dst[i][i3] = dst[tmp_i][i3];
				dst[tmp_i][i3] = tmp;
			}
		}

		f = 1.0f / tempM[i][i];
		for (i2 = 0; i2 < 4; i2++)
		{
			tempM[i][i2] *= f;
			dst[i][i2] *= f;
		}

		for (i3 = 0; i3 < 4; i3++)
		{
			if (i3 == i)
			{
				continue;
			}

			f = tempM[i3][i];
			for (i2 = 0; i2 < 4; i2++)
			{
				tempM[i3][i2] -= tempM[i][i2] * f;
				dst[i3][i2] -= dst[i][i2] * f;
			}
		}
	}

	return 1;
}

/**
 * @note Address: N/A
 * @note Size: 0x4C
 */
void C_MTX44Trans(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x3C
 */
void PSMTX44Trans(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x98
 */
void C_MTX44TransApply(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x5C
 */
void PSMTX44TransApply(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x4C
 */
void C_MTX44Scale(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x34
 */
void PSMTX44Scale(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xB4
 */
void C_MTX44ScaleApply(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x68
 */
void PSMTX44ScaleApply(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x70
 */
void C_MTX44RotRad(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x70
 */
void PSMTX44RotRad(Mtx44 mtx, char axis, f32 radians)
{
	f32 s = sinf(radians);
	f32 c = cosf(radians);
	PSMTX44RotTrig(mtx, axis, s, c);
}

/**
 * @note Address: N/A
 * @note Size: 0x11C
 */
void C_MTX44RotTrig(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xD4
 */
void PSMTX44RotTrig(register Mtx44 m, register char axis, register f32 s, register f32 c)
{
	register f32 one, zero;
	zero = 0.0f;
	one = 1.0f;
#ifdef __MWERKS__ // clang-format off
	asm {
		frsp f6, f1
		ori r0, axis, 0x20
		frsp f5, f2
		cmplwi r0, 'x'
		beq _axis_x
		cmplwi r0, 'y'
		beq _axis_y
		cmplwi r0, 'z'
		beq _axis_z
		b _done
	_axis_x:
		psq_st one, 0x0(m), 1, 0
		psq_st zero, 0x4(m), 0, 0
		ps_neg f0, f6
		psq_st zero, 0xc(m), 0, 0
		ps_merge00 f1, f6, f5
		psq_st zero, 0x1c(m), 0, 0
		ps_merge00 f0, f5, f0
		psq_st zero, 0x2c(m), 0, 0
		psq_st zero, 0x34(m), 0, 0
		psq_st f1, 0x24(m), 0, 0
		psq_st f0, 0x14(m), 0, 0
		psq_st one, 0x3c(m), 1, 0
		b _done
	_axis_y:
		ps_merge00 f1, f5, zero
		psq_st zero, 0x30(m), 0, 0
		ps_neg f0, f6
		psq_st zero, 0x18(m), 0, 0
		ps_merge00 one, zero, one
		psq_st f1, 0x0(m), 0, 0
		ps_merge00 f0, f0, zero
		ps_merge00 f2, f6, zero
		psq_st one, 0x10(m), 0, 0
		psq_st f2, 0x8(m), 0, 0
		psq_st f0, 0x20(m), 0, 0
		psq_st f1, 0x28(m), 0, 0
		psq_st one, 0x38(m), 0, 0
		b _done
	_axis_z:
		psq_st zero, 0x8(m), 0, 0
		ps_neg f0, f6
		psq_st zero, 0x18(m), 0, 0
		ps_merge00 f1, f6, f5
		psq_st zero, 0x20(m), 0, 0
		ps_merge00 f2, one, zero
		psq_st zero, 0x30(m), 0, 0
		ps_merge00 one, zero, one
		psq_st f1, 0x10(m), 0, 0
		ps_merge00 f0, f5, f0
		psq_st f2, 0x28(m), 0, 0
		psq_st one, 0x38(m), 0, 0
		psq_st f0, 0x0(m), 0, 0
	_done:
	}
#endif // clang-format on
}

/**
 * @note Address: N/A
 * @note Size: 0x12C
 */
void C_MTX44RotAxisRad(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0xBC
 */
void __PSMTX44RotAxisRadInternal(void)
{
	// UNUSED FUNCTION
}

/**
 * @note Address: N/A
 * @note Size: 0x70
 */
void PSMTX44RotAxisRad(void)
{
	// UNUSED FUNCTION
}
