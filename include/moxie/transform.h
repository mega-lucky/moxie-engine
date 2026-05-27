#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include "moxmath/moxmath.h"
#include "moxie/world.h"

struct Transform {
    Vec3 Position;
    Vec3 Scale;
    Quat Rotation;
};
typedef struct Transform Transform;
#define TransformIdentity (Transform){ Vec3Zero, Vec3One, QuatIdentity }

Mat4x4 Mat4FromTransform(Transform t);
Mat4x4 Mat4FromTransformNoScale(Transform t);
Vec3 TransformPoint(Vec3 point, Transform transform);

void RegisterTransformComponent();
ComponentMask TransformMask();
Transform *GetTransform(EntityID entity);

#endif