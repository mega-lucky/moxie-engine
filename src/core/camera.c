#include "camera.h"
#include "gamemath.h"
#include "transform.h"

static ComponentMask camera_mask = 0;

Mat4x4 GetViewMatrix(EntityID camera_id) {
    Transform *transform = GetTransform(camera_id);
    if (!transform) { return Mat4Identity; }

    return Mat4Inv(Mat4FromTransformNoScale(*transform));
}
Mat4x4 GetProjectionMatrix(EntityID camera_id) {
    Camera *camera = GetCamera(camera_id);
    if (!camera) { return Mat4Identity; }

    switch (camera->Projection) {
        case PERSPECTIVE_PROJECTION: {
            return PerspectiveMat4(
                camera->Frustrum.FieldOfView,
                camera->Frustrum.AspectRatio,
                camera->Near,
                camera->Far
            );
        }
        case ORTHOGRAPHIC_PROJECTION: {
            return OrthographicMat4(
                camera->Box.Left,
                camera->Box.Right,
                camera->Box.Top,
                camera->Box.Bottom,
                camera->Near,
                camera->Far
            );
        }
        default: return Mat4Identity;
    }
}

void RegisterCameraComponent() {
    camera_mask = RegisterComponent((ComponentDescription){
        .DataSize = sizeof(Camera),
        .DefaultValue = NULL,
        .Destructor = NULL,
    });
}
ComponentMask CameraMask() {
    return camera_mask;
}
Camera *GetCamera(EntityID entity) {
    return GetComponent(entity, camera_mask);
}