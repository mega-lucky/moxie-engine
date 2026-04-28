#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "gamemath.h"
#include "world.h"

struct InputState;

typedef struct ControlState {
    Vec2 CameraRotation;
    Vec3 MoveDirection;
    int MouseLocked;
    int Jumping;
} ControlState;

void UpdateControlState(void *data);

void RegisterControls();
ControlState *GetControls();

#endif