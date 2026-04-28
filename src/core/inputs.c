#include "inputs.h"

#include <string.h>
#include <GLFW/glfw3.h>

static ComponentMask input_state_mask = 0;
static InputState state = {0};

static void synchronise_states() {
    InputState *global_state = GetInputs();
    if (global_state == NULL) { return; }

    memcpy(global_state, &state, sizeof(InputState));
}
static void cursorpos_callback(GLFWwindow *window, double x, double y) {
    state.MouseMovement.x = state.MousePosition.x - x;
    state.MouseMovement.y = state.MousePosition.y - y;
    state.MousePosition.x = x;
    state.MousePosition.y = y;
    state.ActiveInputType = MOUSE_INPUT;
}
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= GLFW_KEY_LAST || key < 0) { return; }
    if (action == GLFW_REPEAT) { return; }

    state.ActiveInputType = KEY_INPUT;
    unsigned int bits = (action == GLFW_PRESS) | (1 << 1);
    state.KeyButtons[key] = bits;

    state.ActiveInputType = KEY_INPUT;
}
static void mousebutton_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button >= GLFW_MOUSE_BUTTON_LAST || button < 0) { return; }
    if (action == GLFW_REPEAT) { return; }
    
    state.ActiveInputType = MOUSE_INPUT;
    uint8_t bits = 1 << (action == GLFW_RELEASE ? 1 : 0);
    if (action != GLFW_RELEASE) { bits |= (1 << 2); }
    state.KeyButtons[button] = bits;
}

void BindInputCallbacks(GLFWwindow *window) {
    glfwSetMouseButtonCallback(window, mousebutton_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
}

void UpdateInputState(void *data) {
    state.MouseMovement.x = 0;
    state.MouseMovement.y = 0;
    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        state.KeyButtons[i] &= ~(1 << 1);
    }
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i ++) {
        state.MouseButtons[i] &= ~(1 << 1);
    }
    glfwPollEvents();
    synchronise_states();
}

void ToggleMouseLocked() {
    GLFWwindow *window = glfwGetCurrentContext();

    int currentmode = glfwGetInputMode(window, GLFW_CURSOR);
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        currentmode == GLFW_CURSOR_DISABLED ?
            GLFW_CURSOR_NORMAL :
            GLFW_CURSOR_DISABLED
    );
}

void RegisterInputState() {
    if (input_state_mask != 0) { return; }
    input_state_mask = RegisterSingleton((ComponentDescription){
        .DataSize = sizeof(InputState),
        .DefaultValue = NULL,
        .Destructor = NULL
    });
}
InputState *GetInputs() {
    return GetSingleton(input_state_mask);
}