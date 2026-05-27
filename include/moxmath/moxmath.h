#ifndef __MOXMATH_MATH__
#define __MOXMATH_MATH__

#include "moxmath/vec2.h"
#include "moxmath/vec3.h"
#include "moxmath/vec4.h"
#include "moxmath/mat4x4.h"
#include "moxmath/quat.h"
#include <math.h>

#define PI 3.141592654
#define PI_F 3.141592654f
#define RADIANS(x) x * 3.141592654 / 180.0
#define DEGREES(x) x * 180.0 / 3.141592654

float lerp(float a, float b, float t);
float fclampf(float x, float min, float max);

#endif