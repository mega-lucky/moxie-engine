#ifndef __MOXMATH_VEC3__
#define __MOXMATH_VEC3__

#define ToVec3(v) (Vec3){.x=(v).x, .y=(v).y, .z=(v).z}
#define Vec3New(X, Y, Z) (Vec3){.x=(X), .y=(Y), .z=(Z)}
#define Vec3Zero (Vec3){.x=0.0f, .y=0.0f,.z=0.0f}
#define Vec3One (Vec3){.x=1.0f,.y=1.0f,.z=1.0f}
#define Vec3XAxis    (Vec3){.x=(1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3Right    (Vec3){.x=(1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3Left     (Vec3){.x=(-1.0f), .y=(0.0f), .z=(0.0f)}
#define Vec3YAxis    (Vec3){.x=(0.0f), .y=(1.0f), .z=(0.0f)}
#define Vec3Up       (Vec3){.x=(0.0f), .y=(1.0f), .z=(0.0f)}
#define Vec3Down     (Vec3){.x=(0.0f), .y=(-1.0f), .z=(0.0f)}
#define Vec3ZAxis    (Vec3){.x=(0.0f), .y=(0.0f), .z=(1.0f)}
#define Vec3Forward  (Vec3){.x=(0.0f), .y=(0.0f), .z=(-1.0f)}
#define Vec3Backward (Vec3){.x=(0.0f), .y=(0.0f), .z=(1.0f)}

Vec3 Vec3Add(Vec3 a, Vec3 b);
Vec3 Vec3Sub(Vec3 a, Vec3 b);
Vec3 Vec3MulV3(Vec3 a, Vec3 b);
Vec3 Vec3MulF(Vec3 a, float f);
Vec3 Vec3MulQ(Vec3 a, Quat quat);
Vec3 Vec3MulM4(Vec3 a, Mat4x4 m);
Vec3 Vec3DivV3(Vec3 a, Vec3 b);
Vec3 Vec3DivF(Vec3 a, float f);
Vec3 Vec3Inv(Vec3 a);
float Vec3Dot(Vec3 a, Vec3 b);
float Vec3Mag(Vec3 a);
Vec3 Vec3Norm(Vec3 a);
float Vec3CosSim(Vec3 a, Vec3 b);
float Vec3Dist(Vec3 a, Vec3 b);
Vec3 Vec3Cross(Vec3 a, Vec3 b);
Vec3 Vec3Lerp(Vec3 a, Vec3 b, float t);
Vec3 Vec3Min(Vec3 a, Vec3 b);
Vec3 Vec3Max(Vec3 a, Vec3 b);
int Vec3Equals(Vec3 a, Vec3 b);

#endif