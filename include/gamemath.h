#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#include <math.h>

union Vec2 {
    float components[2];
    struct { float x, y; };
};
 typedef union Vec2 Vec2;

union Vec3 {
    Vec2 XY;
    
    float components[3];
    struct { float x, y, z; };
};
typedef union Vec3 Vec3;

union Vec4 {
    Vec2 XY;
    Vec3 XYZ;
    
    float components[4];
    struct { float x, y, z, w; };
};
typedef union Vec4 Vec4;

union Mat4x4 {
    float components[16];
    Vec4 columns[4];
    struct {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
};
typedef union Mat4x4 Mat4x4;

#define Mat4GetRow(m, r) (Vec4){ \
    .x=m.components[(r)+ 0], \
    .y=m.components[(r)+ 4], \
    .z=m.components[(r)+ 8], \
    .w=m.components[(r)+12]  \
}

#define PI 3.141592654
#define PI_F 3.141592654f
#define RADIANS(x) x * 3.141592654 / 180.0
#define DEGREES(x) x * 180.0 / 3.141592654

float lerp(float a, float b, float t);
float fclampf(float x, float min, float max);

#define Vec2New(X, Y) (Vec2){.x=(X), .y=(Y)}
#define Vec3New(X, Y, Z) (Vec3){.x=(X), .y=(Y), .z=(Z)}
#define Vec4New(X, Y, Z, W) (Vec4){.x=(X), .y=(Y), .z=(Z), .w=(W)}

#define Vec2Zero (Vec2){.x=0.0f, .y=0.0f}
#define Vec2One (Vec2){.x=1.0f,.y=1.0f}

#define Vec3Zero (Vec3){.x=0.0f, .y=0.0f,.z=0.0f}
#define Vec3One (Vec3){.x=1.0f,.y=1.0f,.z=1.0f}

#define Vec4Zero (Vec4){.x=0.0f, .y=0.0f,.z=0.0f,.w=0.0f}
#define Vec4One (Vec4){.x=1.0f,.y=1.0f,.z=1.0f,.w=1.0f}
#define Vec4Identity (Vec4){.x=0.0f, .y=0.0f, .z=0.0f, .w=1.0f}

#define Vec3XAxis    (Vec3){.x=(1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3Right    (Vec3){.x=(1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3Left     (Vec3){.x=(-1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3YAxis    (Vec3){.x=(0.0f), .y=(1.0f), .z=(0.0f)}
#define Vec3Up       (Vec3){.x=(0.0f), .y=(1.0f), .z=(0.0f)}
#define Vec3Down     (Vec3){.x=(0.0f), .y=(-1.0f), .z=(0.0f)}
#define Vec3ZAxis    (Vec3){.x=(0.0f), .y=(0.0f), .z=(1.0f)}
#define Vec3Forward  (Vec3){.x=(0.0f), .y=(0.0f), .z=(-1.0f)}
#define Vec3Backward (Vec3){.x=(0.0f), .y=(0.0f), .z=(1.0f)}

#define Mat4Identity (Mat4x4){ \
    .m00=1.0f,.m10=0.0f,.m20=0.0f,.m30=0.0f, \
    .m01=0.0f,.m11=1.0f,.m21=0.0f,.m31=0.0f, \
    .m02=0.0f,.m12=0.0f,.m22=1.0f,.m32=0.0f, \
    .m03=0.0f,.m13=0.0f,.m23=0.0f,.m33=1.0f, \
}

Vec2 Vec2Add(Vec2 a, Vec2 b);
Vec2 Vec2Sub(Vec2 a, Vec2 b);
float Vec2Mag(Vec2 a);
Vec3 Vec3Add(Vec3 a, Vec3 b);
Vec3 Vec3Sub(Vec3 a, Vec3 b);
Vec3 Vec3MulV3(Vec3 a, Vec3 b);
Vec3 Vec3MulF(Vec3 a, float f);
Vec3 Vec3MulQ(Vec3 a, Vec4 quat);
Vec3 Vec3MulM4(Vec3 a, Mat4x4 m);
Vec3 Vec3DivV3(Vec3 a, Vec3 b);
Vec3 Vec3DivF(Vec3 a, float f);
Vec3 Vec3Inv(Vec3 a);
float Vec3Dot(Vec3 a, Vec3 b);
float Vec3Mag(Vec3 a);
Vec3 Vec3Norm(Vec3 a);
float Vec3NDot(Vec3 a, Vec3 b);
float Vec3Dist(Vec3 a, Vec3 b);
Vec3 Vec3Cross(Vec3 a, Vec3 b);
Vec3 Vec3Lerp(Vec3 a, Vec3 b, float t);
Vec3 Vec3Min(Vec3 a, Vec3 b);
Vec3 Vec3Max(Vec3 a, Vec3 b);
int Vec3Equals(Vec3 a, Vec3 b);
Vec4 Vec4FromV3(Vec3 v);
float Vec4Dot(Vec4 a, Vec4 b);
float Vec4Mag(Vec4 a);
Vec4 Vec4Norm(Vec4 a);

Vec4 Quaternion(float x, float y, float z, float w);
Vec4 QuatFromAxisAngle(Vec3 axis, float angle);
Vec4 QuatMul(Vec4 q2, Vec4 q1);
Vec4 QuatInv(Vec4 q);
Vec4 Vec4MulM4(Vec4 a, Mat4x4 m);
Vec4 Vec4MulQ(Vec4 a, Vec4 q);

Mat4x4 Mat4Transpose(Mat4x4 a);
Mat4x4 Mat4Mul(Mat4x4 a, Mat4x4 b);
Mat4x4 TranslateMat4(float x, float y, float z);
Mat4x4 ScaleMat4(float x, float y, float z);
Mat4x4 XRotMat4(float angle);
Mat4x4 YRotMat4(float angle);
Mat4x4 ZRotMat4(float angle);
Mat4x4 RotMat4Euler(float x, float y, float z);
Mat4x4 RotMat4Quat(Vec4 quat);
Mat4x4 PerspectiveMat4(float fov, float aspect_ratio, float near, float far);
Mat4x4 OrthographicMat4(float left, float right, float top, float bottom, float near, float far);
Mat4x4 Mat4Offset(Mat4x4 a, Vec3 offset);
Mat4x4 Mat4FromVecs(Vec3 pos, Vec3 xv, Vec3 yv, Vec3 zv);
Mat4x4 Mat4Inv(Mat4x4 a);
Vec3 Mat4Position(Mat4x4 a);
Vec3 Mat4RightVec(Mat4x4 a);
Vec3 Mat4UpVec(Mat4x4 a);
Vec3 Mat4LookVec(Mat4x4 a);
Mat4x4 Mat4Rotation(Mat4x4 a);
Mat4x4 Mat4LookAt(Vec3 from, Vec3 to);
Vec4 Mat4MulV4(Mat4x4 a, Vec4 v);

#endif