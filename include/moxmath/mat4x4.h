#ifndef __MOXMATH_MAT4x4__
#define __MOXMATH_MAT4x4__

#include "moxmath/types.h"

#define Mat4Identity (Mat4x4){ \
    .m00=1.0f,.m10=0.0f,.m20=0.0f,.m30=0.0f, \
    .m01=0.0f,.m11=1.0f,.m21=0.0f,.m31=0.0f, \
    .m02=0.0f,.m12=0.0f,.m22=1.0f,.m32=0.0f, \
    .m03=0.0f,.m13=0.0f,.m23=0.0f,.m33=1.0f, \
}

#define Mat4GetRow(m, r) (Vec4){ \
    .x=m.components[(r)+ 0], \
    .y=m.components[(r)+ 4], \
    .z=m.components[(r)+ 8], \
    .w=m.components[(r)+12]  \
}

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