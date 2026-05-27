#ifndef __MOXMATH_TYPES__
#define __MOXMATH_TYPES__

typedef union Vec2 {
    float components[2];
    struct { float x, y; };
} Vec2;

typedef union Vec3 {
    float components[3];
    struct { float x, y, z; };
} Vec3;

typedef union Vec4 {
    float components[4];
    struct { float x, y, z, w; };
} Vec4;

typedef union Quat {
    float components[4];
    struct { float x, y, z, w; };
} Quat;

typedef union Mat4x4 {
    float components[16];
    Vec4 columns[4];
    struct {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
} Mat4x4;

#endif