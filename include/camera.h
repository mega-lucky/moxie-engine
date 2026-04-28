#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include "gamemath.h"
#include "world.h"

enum ProjectionType {
    PERSPECTIVE_PROJECTION,
    ORTHOGRAPHIC_PROJECTION,
    PROJECTION_NONE
};

enum CameraMode {
    FREE_CAMERA,
    THIRD_PERSON,
    FIRST_PERSON,
    CAMERAMODE_NONE
};

typedef struct CameraFrustrum {
    float FieldOfView;
    float AspectRatio;
} CameraFrustrum;

typedef struct CameraBox {
    float Left, Right;
    float Top, Bottom;
} CameraBox;

typedef struct Camera {
    enum ProjectionType Projection;
    float Near, Far;
    union {
        CameraFrustrum Frustrum;
        CameraBox Box;
    };
} Camera;

typedef struct CameraSystemState {
    EntityQuery *cameras;
} CameraSystemState;

void CameraSystemInit(CameraSystemState *state);
void CameraSystemUpdate(CameraSystemState *state);

Mat4x4 GetViewMatrix(EntityID camera);
Mat4x4 GetProjectionMatrix(EntityID camera);

void RegisterCameraComponent();
ComponentMask CameraMask();
Camera *GetCamera(EntityID entity);

#endif