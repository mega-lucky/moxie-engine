#ifndef __MOXMATH_VEC2__
#define __MOXMATH_VEC2__

#include "moxmath/types.h"

#define Vec2New(X, Y) (Vec2){.x=(X), .y=(Y)}

#define Vec2Zero (Vec2){.x=0.0f, .y=0.0f}
#define Vec2One (Vec2){.x=1.0f,.y=1.0f}

Vec2 Vec2Add(Vec2 a, Vec2 b);
Vec2 Vec2Sub(Vec2 a, Vec2 b);
float Vec2Mag(Vec2 a);

#endif