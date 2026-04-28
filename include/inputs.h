#ifndef INPUTHANDLERS_H
#define INPUTHANDLERS_H

#include <stdint.h>

typedef uint8_t InputInfo;

#include "gamemath.h"
#include "world.h"

enum InputType {
    INPUTTYPE_NONE,
    KEY_INPUT,
    MOUSE_INPUT,
};

#define N_KEYS 348
#define N_MOUSEBUTTONS 7

#define INPUT_DOWN_MASK    (1 << 0)
#define INPUT_CHANGED_MASK (1 << 1)

#define IS_INPUT_DOWN(x)    ((x & INPUT_DOWN_MASK) != 0)
#define IS_INPUT_CHANGED(x) ((x & INPUT_CHANGED_MASK) != 0)
#define IS_INPUT_STARTED(x) (IS_INPUT_DOWN(x) && IS_INPUT_CHANGED(x))
#define IS_INPUT_ENDED(x)   (!IS_INPUT_DOWN(x) && IS_INPUT_CHANGED(x))

typedef struct InputState {
    InputInfo KeyButtons[N_KEYS];
    InputInfo MouseButtons[N_MOUSEBUTTONS];
    enum InputType ActiveInputType;
    Vec2 MousePosition;
    Vec2 MouseMovement;
} InputState;

struct GLFWwindow;
void BindInputCallbacks(struct GLFWwindow *window);
void UpdateInputState(void *data);
void ToggleMouseLocked();

void RegisterInputState();
InputState *GetInputs();

#endif