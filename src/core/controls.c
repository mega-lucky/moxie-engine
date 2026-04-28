#include "controls.h"
#include "inputs.h"

static ComponentMask controls_mask = 0;

static void interpret_kbm(InputState *input_state, ControlState *control_state) {
    if (IS_INPUT_STARTED(input_state->KeyButtons['U'])) {
        control_state->MouseLocked = !control_state->MouseLocked;
        ToggleMouseLocked();
    }
    Vec2 mouse_delta = input_state->MouseMovement;
    if (control_state->MouseLocked) {
        control_state->CameraRotation.x -= RADIANS(mouse_delta.y * 0.5f);
        control_state->CameraRotation.y -= RADIANS(mouse_delta.x * 0.5f);

        if (control_state->CameraRotation.y < PI_F) control_state->CameraRotation.y += 2.0f * PI_F;
        if (control_state->CameraRotation.y > PI_F) control_state->CameraRotation.y -= 2.0f * PI_F;
    }

    control_state->MoveDirection.x = IS_INPUT_DOWN(input_state->KeyButtons['D']) -
        IS_INPUT_DOWN(input_state->KeyButtons['A']);

    control_state->MoveDirection.z = IS_INPUT_DOWN(input_state->KeyButtons['W']) -
        IS_INPUT_DOWN(input_state->KeyButtons['S']);
    
    control_state->MoveDirection = Vec3Norm(control_state->MoveDirection);
    control_state->Jumping = IS_INPUT_DOWN(input_state->KeyButtons[' ']);
}

void UpdateControlState(void *data) {
    InputState *input_state = GetInputs();
    ControlState *control_state = GetControls();
    if (!input_state || !control_state) { return; }

    switch (input_state->ActiveInputType) {
        case MOUSE_INPUT: case KEY_INPUT: {
            interpret_kbm(input_state, control_state); break;
        }
        default: {}
    }
}

void RegisterControls() {
    if (controls_mask != 0) { return; }
    controls_mask = RegisterSingleton((ComponentDescription){
        .DataSize = sizeof(ControlState),
        .DefaultValue = NULL,
        .Destructor = NULL
    });
}
ControlState *GetControls() {
    return GetSingleton(controls_mask);
}