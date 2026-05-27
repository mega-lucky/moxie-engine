#ifndef __MOXMATH_QUAT__
#define __MOXMATH_QUAT__

#include "moxmath/types.h"

Quat Quaternion(float x, float y, float z, float w);
Quat QuatFromAxisAngle(Vec3 axis, float angle);
Quat QuatMul(Quat q2, Quat q1);
Quat QuatInv(Quat q);
Quat QuatMulM4(Quat a, Mat4x4 m);
Quat QuatMulQ(Quat a, Quat q);
Vec4 QuatAsVec4(Quat a);
Quat QuatNorm(Quat a);

#define QuatIdentity (Quat){.x=0.0f, .y=0.0f, .z=0.0f, .w=1.0f}

#endif