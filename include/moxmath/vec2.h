#ifndef __MOXMATH_VEC2__
#define __MOXMATH_VEC2__

#include "moxmath/types.h"

#define Vec2New(X, Y) (Vec2){.x=(X), .y=(Y)}

#define Vec2Zero (Vec2){.x=0.0f, .y=0.0f}
#define Vec2One (Vec2){.x=1.0f,.y=1.0f}

Vec2 Vec2Add(Vec2 a, Vec2 b);
Vec2 Vec2Sub(Vec2 a, Vec2 b);
Vec2 Vec2MulV2(Vec2 a, Vec2 b);
Vec2 Vec2MulF(Vec2 a, float b);
Vec2 Vec2DivV2(Vec2 a, Vec2 b);
Vec2 Vec2DivF(Vec2 a, float b);
Vec2 Vec2Inv(Vec2 a);
float Vec2Dot(Vec2 a, Vec2 b);
float Vec2Mag(Vec2 a);
Vec2 Vec2Norm(Vec2 a);
float Vec2CosSim(Vec2 a, Vec2 b);
float Vec2Dist(Vec2 a, Vec2 b);
Vec2 Vec2Lerp(Vec2 a, Vec2 b, float t);
Vec2 Vec2Min(Vec2 a, Vec2 b);
Vec2 Vec2Max(Vec2 a, Vec2 b);
int Vec2Equals(Vec2 a, Vec2 b);

#endif