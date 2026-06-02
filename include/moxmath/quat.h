#ifndef __MOXMATH_QUAT__
#define __MOXMATH_QUAT__

#include "moxmath/types.h"

Quat Quaternion(float x, float y, float z, float w);
Quat QuatFromAxisAngle(Vec3 axis, float angle);
Quat QuatMulQ(Quat a, Quat q);
Quat QuatInv(Quat q);
Vec4 QuatAsVec4(Quat a);
Quat QuatNorm(Quat a);

#define QuatIdentity (Quat){.x=0.0f, .y=0.0f, .z=0.0f, .w=1.0f}

#endif