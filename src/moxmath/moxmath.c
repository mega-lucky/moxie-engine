#include "moxmath/moxmath.h"

float lerp(float a, float b, float t) {
    return fmaf(t, b, fmaf(-t, a, a));
}

float fclampf(float x, float min, float max) {
    return fmaxf(fminf(x, max), min);
}

Vec2 Vec2Add(Vec2 a, Vec2 b) {
    return (Vec2) {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}
Vec2 Vec2Sub(Vec2 a, Vec2 b) {
    return (Vec2) {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}
float Vec2Mag(Vec2 a) {
    return sqrt(a.x*a.x + a.y*a.y);
}
Vec3 Vec3Add(Vec3 a, Vec3 b) {
    return (Vec3) {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}
Vec3 Vec3Sub(Vec3 a, Vec3 b) {
    return (Vec3) {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}
Vec3 Vec3MulV3(Vec3 a, Vec3 b) {
    return (Vec3) {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z
    };
}
Vec3 Vec3MulF(Vec3 a, float f) {
    return (Vec3) {
        .x = a.x * f,
        .y = a.y * f,
        .z = a.z * f
    };
}
Vec3 Vec3MulQ(Vec3 a, Quat quat) {
    return ToVec3(Vec4MulQ(Vec4FromV3(a), quat));
}
Vec3 Vec3MulM4(Vec3 a, Mat4x4 m) {
    return ToVec3(Vec4MulM4(Vec4FromV3(a), m));
}
Vec3 Vec3DivV3(Vec3 a, Vec3 b) {
    return (Vec3) {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z
    };
}

Vec3 Vec3DivF(Vec3 a, float f) {
    return (Vec3) {
        .x = a.x / f,
        .y = a.y / f,
        .z = a.z / f
    };
}

Vec3 Vec3Inv(Vec3 a) {
    return (Vec3) { .x = -a.x, .y = -a.y, .z = -a.z };
}

float Vec3Dot(Vec3 a, Vec3 b) {
    return fmaf(a.x, b.x, fmaf(a.y, b.y, a.z * b.z));
}

float Vec3Mag(Vec3 a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 Vec3Norm(Vec3 a) {
    float Mag = Vec3Mag(a);
    if (Mag == 0.0f) { return (Vec3){0}; }
    return Vec3DivF(a, Mag);
}

float Vec3NDot(Vec3 a, Vec3 b) {
    return Vec3Dot(Vec3Norm(a), Vec3Norm(b));
}

float Vec3Dist(Vec3 a, Vec3 b) {
    return Vec3Mag(Vec3Sub(a, b));
}

Vec3 Vec3Cross(Vec3 a, Vec3 b) {
    float x = (a.y * b.z) - (b.y * a.z);
    float y = (a.x * b.z) - (b.x * a.z);
    float z = (a.x * b.y) - (b.x * a.y);
    
    return (Vec3){.x = x, .y = 0.0f-y, .z = z};
}

Vec3 Vec3Lerp(Vec3 a, Vec3 b, float t) {
    return (Vec3) {
        .x = lerp(a.x, b.x, t),
        .y = lerp(a.y, b.y, t),
        .z = lerp(a.z, b.z, t),
    };
}

Vec3 Vec3Min(Vec3 a, Vec3 b) {
    return (Vec3) {
        .x = fminf(a.x, b.x),
        .y = fminf(a.y, b.y),
        .z = fminf(a.z, b.z),
    };
}

Vec3 Vec3Max(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = fmaxf(a.x, b.x),
        .y = fmaxf(a.y, b.y),
        .z = fmaxf(a.z, b.z),
    };
}

int Vec3Equals(Vec3 a, Vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
Vec4 Vec4FromV3(Vec3 v) {
    return Vec4New(v.x,v.y,v.z,0.0f);
}
float Vec4Dot(Vec4 a, Vec4 b) {
    return fmaf(a.x, b.x, fmaf(a.y, b.y, fmaf(a.z, b.z, a.w * b.w)));
}
float Vec4Mag(Vec4 a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
}
Vec4 Vec4Norm(Vec4 a) {
    Vec4 result = a;

    float mag = Vec4Mag(a);
    if (mag == 0.0f) { return (Vec4){0}; }

    for (int i = 0; i < 4; i ++) { result.components[i] /= mag; }

    return result;
}
Quat Vec4AsQuat(Vec4 a) {
    union { Quat q; Vec4 v; } u = { .v = a };
    return u.q;
}

Quat Quaternion(float x, float y, float z, float w) {
    float half = w / 2.0f;
    
    Vec4 result = Vec4New(
        x * sinf(half),
        y * sinf(half),
        z * sinf(half),
        cosf(half)
    );

    return Vec4AsQuat(Vec4Norm(result));
}

Quat QuatFromAxisAngle(Vec3 axis, float angle) {
    return Quaternion(axis.x, axis.y, axis.z, angle);
}

Quat QuatMul(Quat q2, Quat q1) {
    Quat result = {
        .x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        .y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        .z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
        .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    };
    return result;
}

Quat QuatInv(Quat q) {
    float mag2 = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;

    return Vec4AsQuat(Vec4New(-q.x / mag2, -q.y / mag2, -q.z / mag2, q.w / mag2));
}

Quat QuatNorm(Quat a) {
    Quat result = a;

    float mag = Vec4Mag(QuatAsVec4(a));
    if (mag == 0.0f) { return (Quat){0}; }

    for (int i = 0; i < 4; i ++) { result.components[i] /= mag; }

    return result;
}

Vec4 QuatAsVec4(Quat a) {
    union { Quat q; Vec4 v; } u = { .q = a };
    return u.v;
}

Vec4 Vec4MulM4(Vec4 a, Mat4x4 m) {
    return (Vec4){
        .x = a.x * m.m00 + a.y * m.m01 + a.z * m.m02 + a.w * m.m03,
        .y = a.x * m.m10 + a.y * m.m11 + a.z * m.m12 + a.w * m.m13,
        .z = a.x * m.m20 + a.y * m.m21 + a.z * m.m22 + a.w * m.m23,
        .w = a.x * m.m30 + a.y * m.m31 + a.z * m.m32 + a.w * m.m33,
    };
}

Vec4 Vec4MulQ(Vec4 a, Quat q) {
    return QuatAsVec4(
        QuatMul(q, QuatMul(Vec4AsQuat(a), QuatInv(q)))
    );
}

Mat4x4 Mat4Transpose(Mat4x4 a) {
    Mat4x4 transposed = {
        .m00 = a.m00, .m01 = a.m10, .m02 = a.m20, .m03 = a.m30,
        .m10 = a.m01, .m11 = a.m11, .m12 = a.m21, .m13 = a.m31,
        .m20 = a.m02, .m21 = a.m12, .m22 = a.m22, .m23 = a.m32,
        .m30 = a.m03, .m31 = a.m13, .m32 = a.m23, .m33 = a.m33,
    };
    return transposed;
}

Mat4x4 Mat4Mul(Mat4x4 a, Mat4x4 b) {
    Mat4x4 result;

    for (int row = 0; row < 4; row ++) {
        for (int col = 0; col < 4; col ++) {
            Vec4 v1 = Mat4GetRow(a, row);
            Vec4 v2 = b.columns[col];

            result.components[col * 4 + row] = Vec4Dot(v1, v2);
        }
    }
    
    return result;
}

Mat4x4 TranslateMat4(float x, float y, float z) {
    Mat4x4 result = Mat4Identity;
    result.m03 = x;
    result.m13 = y;
    result.m23 = z;
    return result;
}

Mat4x4 ScaleMat4(float x, float y, float z) {
    Mat4x4 result = Mat4Identity;

    result.m00 = x;
    result.m11 = y;
    result.m22 = z;

    return result;
}

Mat4x4 XRotMat4(float angle) {
    Mat4x4 result;

    result.m00  = 1.0f; result.m01  = 0.0f;        result.m02  = 0.0f;         result.m03  = 0.0f;
    result.m10  = 0.0f; result.m11  = cosf(angle); result.m12  = -sinf(angle); result.m13  = 0.0f;
    result.m20  = 0.0f; result.m21  = sinf(angle); result.m22 = cosf(angle);   result.m23 = 0.0f;
    result.m30 = 0.0f;  result.m31 = 0.0f;         result.m32 = 0.0f;          result.m33 = 1.0f;

    return result;
}

Mat4x4 YRotMat4(float angle) {
    Mat4x4 result;

    result.m00 = cosf(angle);  result.m01 = 0.0f; result.m02 = sinf(angle); result.m03 = 0.0f;
    result.m10 = 0.0f;         result.m11 = 1.0f; result.m12 = 0.0f;        result.m13 = 0.0f;
    result.m20 = -sinf(angle); result.m21 = 0.0f; result.m22 = cosf(angle); result.m23 = 0.0f;
    result.m30 = 0.0f;         result.m31 = 0.0f; result.m32 = 0.0f;        result.m33 = 1.0f;

    return result;
}

Mat4x4 ZRotMat4(float angle) {
    Mat4x4 result;

    result.m00  = cosf(angle); result.m01  = -sinf(angle); result.m02  = 0.0f; result.m03  = 0.0f;
    result.m10  = sinf(angle); result.m11  = cosf(angle);  result.m12  = 0.0f; result.m13  = 0.0f;
    result.m20  = 0.0f;        result.m21  = 0.0f;         result.m22 = 1.0f;  result.m23 = 0.0f;
    result.m30 = 0.0f;         result.m31 = 0.0f;          result.m32 = 0.0f;  result.m33 = 1.0f;

    return result;
}

Mat4x4 RotMat4Euler(float x, float y, float z) {
    Mat4x4 Xrot = XRotMat4(x);
    Mat4x4 Yrot = YRotMat4(y);
    Mat4x4 Zrot = ZRotMat4(z);

    Mat4x4 rotated = Mat4Mul(Yrot, Xrot);
    return Mat4Mul(rotated, Zrot);
}

Mat4x4 RotMat4Quat(Vec4 quat) {
    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;

    Mat4x4 Rot;
    Rot.m00 = 1.0f - 2.0f * (y*y + z*z);
    Rot.m10 = 2.0f * (x*y - w*z);
    Rot.m20 = 2.0f * (x*z + w*y);
    Rot.m30 = 0.0f;

    Rot.m01 = 2.0f * (y*x + w*z);
    Rot.m11 = 1.0f - 2.0f * (x*x + z*z);
    Rot.m21 = 2.0f * (y*z - w*x);
    Rot.m31 = 0.0f;
    
    Rot.m02 = 2.0f * (z*x - w*y);
    Rot.m12 = 2.0f * (z*y + w*x);
    Rot.m22 = 1.0f - 2.0f * (x*x + y*y);
    Rot.m32 = 0.0f;
    
    Rot.m03 = 0.0f;
    Rot.m13 = 0.0f;
    Rot.m23 = 0.0f;
    Rot.m33 = 1.0f;

    return Rot;
}

Mat4x4 PerspectiveMat4(float fov, float aspect_ratio, float near, float far) {
    Mat4x4 result = {0};

    result.m00 = 1/(aspect_ratio * tanf(fov/2));
    result.m11 = 1/tanf(fov/2);
    result.m22 = -(far+near)/(far-near);
    result.m32 = -1.0f;
    result.m23 = -2.0f * far * near / (far - near);

    return result;
}

Mat4x4 OrthographicMat4(float left, float right, float top, float bottom, float near, float far) {
    Mat4x4 result = {0};

    result.m00 = 2.0f / (right - left);
    result.m11 = 2.0f / (top - bottom);
    result.m22 = -2.0f / (far - near);
    result.m03 = -(right + left) / (right - left);
    result.m13 = -(top + bottom) / (top - bottom);
    result.m23 = -(far + near) / (far - near);
    result.m33 = 1.0f;

    return result;
}

Mat4x4 Mat4Offset(Mat4x4 a, Vec3 offset) {
    Mat4x4 result;

    for (int i = 0; i < 16; i ++) {
        result.components[i] = a.components[i];
    }

    result.m03 += offset.x;
    result.m13 += offset.y;
    result.m23 += offset.z;

    return result;
}

Mat4x4 Mat4FromVecs(Vec3 pos, Vec3 xv, Vec3 yv, Vec3 zv) {
    xv = Vec3Norm(xv), yv = Vec3Norm(yv), zv = Vec3Norm(zv);
    Mat4x4 result;
    
    result.m00 = xv.x; result.m01 = yv.x; result.m02 = zv.x; result.m03 = pos.x;
    result.m10 = xv.y; result.m11 = yv.y; result.m12 = zv.y; result.m13 = pos.y;
    result.m20 = xv.z; result.m21 = yv.z; result.m22 = zv.z; result.m23 = pos.z;
    result.m30 = 0.0f; result.m31 = 0.0f; result.m32 = 0.0f;  result.m33 = 1.0f;

    return result;
}

static void swap_rows(float aug[32], int first_row, int second_row) {
    float temp[8];
    for (int i = 0; i < 8; i ++) {
        temp[i] = aug[i * 4 + second_row];
    }

    for (int i = 0; i < 8; i ++) {
        aug[i * 4 + second_row] = aug[i * 4 + first_row];
    }

    for (int i = 0; i < 8; i ++) {
        aug[i * 4 + first_row] = temp[i];
    }
}

Mat4x4 Mat4Inv(Mat4x4 a) {
    Mat4x4 result = {0};

    float aug[32] = {0};
    for (int i = 0; i < 16; i ++) {
        aug[i] = a.components[i];
        aug[i+16] = i % 5 == 0 ? 1.0f : 0.0f;
    }
    
    for (int i = 0; i < 4; i ++) {
        int pivot_row = i;
        float pivot = aug[i * 4 + i];
        while (pivot == 0.0f && (pivot_row) < 4) {
            pivot_row ++;
            pivot = aug[i * 4 + pivot_row];
        }

        if (pivot == 0.0f) { continue; }
        else if (pivot_row != i) { swap_rows(aug, i, pivot_row); }

        for (int j = i; j < 8; j ++) {
            aug[j*4+i] /= pivot;
        }

        for (int j = 0; j < 4; j ++) {
            if (i == j) { continue; }
            float factor = aug[i*4+j];
            for (int k = 0; k < 8; k ++) {
                aug[k*4+j] -= factor * aug[k*4+i];
            }
        }
    }

    for (int i = 0; i < 16; i ++) {
        result.components[i] = aug[i + 16];
    }

    return result;
}

Vec3 Mat4Position(Mat4x4 a) {
    Vec3 position = {.x = a.m03, .y = a.m13, .z = a.m23};
    return position;
}

Vec3 Mat4RightVec(Mat4x4 a) {
    Vec3 right_Vec = {.x = a.m00, .y = a.m10, .z = a.m20};
    return Vec3Norm(right_Vec);
}

Vec3 Mat4UpVec(Mat4x4 a) {
    Vec3 up_Vec = {.x = a.m01, .y = a.m11, .z = a.m21};
    return Vec3Norm(up_Vec);
}

Vec3 Mat4LookVec(Mat4x4 a) {
    Vec3 look_Vec = {.x = a.m02, .y = a.m12, .z = a.m22};
    return Vec3Norm(look_Vec);
}

Mat4x4 Mat4Rotation(Mat4x4 a) {
    Mat4x4 result;
    for (int i = 0; i < 16; i ++) {
        result.components[i] = i >= 12 && i <= 14 ? 0.0f : a.components[i];
    }
    return result;
}

Vec4 Mat4MulV4(Mat4x4 a, Vec4 v) {
    Vec4 result;

    for (int i = 0; i < 4; i ++) {
        result.components[i] = Vec4Dot(Mat4GetRow(a, i), v);
    }

    return result;
}

Mat4x4 Mat4LookAt(Vec3 from, Vec3 to) {
    Vec3 lookVec = Vec3Norm(Vec3Sub(to, from));
    Vec3 rightVec = Vec3Cross(Vec3New(0.0f,1.0f,0.0f), lookVec);
    Vec3 upVec = Vec3Cross(lookVec, rightVec);

    return Mat4FromVecs(from, rightVec, upVec, lookVec);
}