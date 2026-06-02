#ifndef __MOXMATH_VEC4__
#define __MOXMATH_VEC4__

#include "moxmath/types.h"

#define Vec4New(X, Y, Z, W) (Vec4){.x=(X), .y=(Y), .z=(Z), .w=(W)}
#define Vec4Zero (Vec4){.x=0.0f, .y=0.0f,.z=0.0f,.w=0.0f}
#define Vec4One (Vec4){.x=1.0f,.y=1.0f,.z=1.0f,.w=1.0f}
#define Vec4Identity (Vec4){.x=0.0f, .y=0.0f, .z=0.0f, .w=1.0f}

Vec4 Vec4Add(Vec4 a, Vec4 b);
Vec4 Vec4Sub(Vec4 a, Vec4 b);
Vec4 Vec4MulV4(Vec4 a, Vec4 b);
Vec4 Vec4MulF(Vec4 a, float f);
Vec4 Vec4MulM4(Vec4 a, Mat4x4 m);
Vec4 Vec4MulQ(Vec4 a, Quat q);
Vec4 Vec4DivV4(Vec4 a, Vec4 b);
Vec4 Vec4DivF(Vec4 a, float f);
Vec4 Vec4Inv(Vec4 a);
float Vec4Dot(Vec4 a, Vec4 b);
float Vec4Mag(Vec4 a);
Vec4 Vec4Norm(Vec4 a);
float Vec4CosSim(Vec4 a, Vec4 b);
float Vec4Dist(Vec4 a, Vec4 b);
Vec4 Vec4Lerp(Vec4 a, Vec4 b, float t);
Vec4 Vec4Min(Vec4 a, Vec4 b);
Vec4 Vec4Max(Vec4 a, Vec4 b);
int Vec4Equals(Vec4 a, Vec4 b);
Quat Vec4AsQuat(Vec4 a);
Vec4 Vec4FromV3(Vec3 v);

#endif