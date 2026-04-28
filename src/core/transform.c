#include "transform.h"

static ComponentMask transform_mask = 0;

Mat4x4 Mat4FromTransform(Transform t) {
    t.Rotation = Vec4Norm(t.Rotation);
    float rx = t.Rotation.x;
    float ry = t.Rotation.y;
    float rz = t.Rotation.z;
    float rw = t.Rotation.w;

    float px = t.Position.x;
    float py = t.Position.y;
    float pz = t.Position.z;

    float sx = t.Scale.x;
    float sy = t.Scale.y;
    float sz = t.Scale.z;

    Mat4x4 Rot;
    Rot.m00 = sx * (1.0f - 2.0f * (ry*ry + rz*rz));
    Rot.m10 = sx * 2.0f * (rx*ry - rw*rz);
    Rot.m20 = sx * 2.0f * (rx*rz + rw*ry);
    Rot.m30 = 0.0f;

    Rot.m01 = sy * 2.0f * (ry*rx + rw*rz);
    Rot.m11 = sy * (1.0f - 2.0f * (rx*rx + rz*rz));
    Rot.m21 = sy * 2.0f * (ry*rz - rw*rx);
    Rot.m31 = 0.0f;
    
    Rot.m02 = sz * 2.0f * (rz*rx - rw*ry);
    Rot.m12 = sz * 2.0f * (rz*ry + rw*rx);
    Rot.m22 = sz * (1.0f - 2.0f * (rx*rx + ry*ry));
    Rot.m32 = 0.0f;
    
    Rot.m03 = px;
    Rot.m13 = py;
    Rot.m23 = pz;
    Rot.m33 = 1.0f;

    return Rot;
}
Mat4x4 Mat4FromTransformNoScale(Transform t) {
    t.Rotation = Vec4Norm(t.Rotation);
    float rx = t.Rotation.x;
    float ry = t.Rotation.y;
    float rz = t.Rotation.z;
    float rw = t.Rotation.w;

    float px = t.Position.x;
    float py = t.Position.y;
    float pz = t.Position.z;

    Mat4x4 Rot;
    Rot.m00 = (1.0f - 2.0f * (ry*ry + rz*rz));
    Rot.m10 = 2.0f * (rx*ry - rw*rz);
    Rot.m20 = 2.0f * (rx*rz + rw*ry);
    Rot.m30 = 0.0f;

    Rot.m01 = 2.0f * (ry*rx + rw*rz);
    Rot.m11 = (1.0f - 2.0f * (rx*rx + rz*rz));
    Rot.m21 = 2.0f * (ry*rz - rw*rx);
    Rot.m31 = 0.0f;
    
    Rot.m02 = 2.0f * (rz*rx - rw*ry);
    Rot.m12 = 2.0f * (rz*ry + rw*rx);
    Rot.m22 = (1.0f - 2.0f * (rx*rx + ry*ry));
    Rot.m32 = 0.0f;
    
    Rot.m03 = px;
    Rot.m13 = py;
    Rot.m23 = pz;
    Rot.m33 = 1.0f;

    return Rot;
}
Vec3 TransformPoint(Vec3 point, Transform transform) {
    Vec3 result = Vec3MulV3(point, transform.Scale);
    result = Vec3MulQ(result, transform.Rotation);
    result = Vec3Add(result, transform.Position);

    return result;
}
void RegisterTransformComponent() {
    if (transform_mask != 0) { return; }
    transform_mask = RegisterComponent((ComponentDescription){
        .DataSize = sizeof(Transform),
        .DefaultValue = &TransformIdentity,
        .Destructor = NULL
    });
}

ComponentMask TransformMask() {
    return transform_mask;
}

Transform *GetTransform(EntityID entity) {
    return GetComponent(entity, transform_mask);
}