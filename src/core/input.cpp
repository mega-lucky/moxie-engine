#include <input.hpp>
#include <glfwinternal.hpp>
#include <GLFW/glfw3.h>

static Input::KeyInput glfw_tokey[GLFW_KEY_LAST + 1] = { Input::KeyInput::Unknown };
static Input::MouseInput glfw_tomouse[GLFW_MOUSE_BUTTON_LAST + 1] = { Input::MouseInput::Left };
static bool mapping_initalised = false;


static void init_input_maps() {
    if (mapping_initalised) return;

    glfw_tokey[GLFW_KEY_A] = Input::KeyInput::A;
    glfw_tokey[GLFW_KEY_B] = Input::KeyInput::B;
    glfw_tokey[GLFW_KEY_C] = Input::KeyInput::C;
    glfw_tokey[GLFW_KEY_D] = Input::KeyInput::D;
    glfw_tokey[GLFW_KEY_E] = Input::KeyInput::E;
    glfw_tokey[GLFW_KEY_F] = Input::KeyInput::F;
    glfw_tokey[GLFW_KEY_G] = Input::KeyInput::G;
    glfw_tokey[GLFW_KEY_H] = Input::KeyInput::H;
    glfw_tokey[GLFW_KEY_I] = Input::KeyInput::I;
    glfw_tokey[GLFW_KEY_J] = Input::KeyInput::J;
    glfw_tokey[GLFW_KEY_K] = Input::KeyInput::K;
    glfw_tokey[GLFW_KEY_L] = Input::KeyInput::L;
    glfw_tokey[GLFW_KEY_M] = Input::KeyInput::M;
    glfw_tokey[GLFW_KEY_N] = Input::KeyInput::N;
    glfw_tokey[GLFW_KEY_O] = Input::KeyInput::O;
    glfw_tokey[GLFW_KEY_P] = Input::KeyInput::P;
    glfw_tokey[GLFW_KEY_Q] = Input::KeyInput::Q;
    glfw_tokey[GLFW_KEY_R] = Input::KeyInput::R;
    glfw_tokey[GLFW_KEY_S] = Input::KeyInput::S;
    glfw_tokey[GLFW_KEY_T] = Input::KeyInput::T;
    glfw_tokey[GLFW_KEY_U] = Input::KeyInput::U;
    glfw_tokey[GLFW_KEY_V] = Input::KeyInput::V;
    glfw_tokey[GLFW_KEY_W] = Input::KeyInput::W;
    glfw_tokey[GLFW_KEY_X] = Input::KeyInput::X;
    glfw_tokey[GLFW_KEY_Y] = Input::KeyInput::Y;
    glfw_tokey[GLFW_KEY_Z] = Input::KeyInput::Z;

    glfw_tokey[GLFW_KEY_0] = Input::KeyInput::Num0;
    glfw_tokey[GLFW_KEY_1] = Input::KeyInput::Num1;
    glfw_tokey[GLFW_KEY_2] = Input::KeyInput::Num2;
    glfw_tokey[GLFW_KEY_3] = Input::KeyInput::Num3;
    glfw_tokey[GLFW_KEY_4] = Input::KeyInput::Num4;
    glfw_tokey[GLFW_KEY_5] = Input::KeyInput::Num5;
    glfw_tokey[GLFW_KEY_6] = Input::KeyInput::Num6;
    glfw_tokey[GLFW_KEY_7] = Input::KeyInput::Num7;
    glfw_tokey[GLFW_KEY_8] = Input::KeyInput::Num8;
    glfw_tokey[GLFW_KEY_9] = Input::KeyInput::Num9;

    glfw_tokey[GLFW_KEY_ESCAPE]        = Input::KeyInput::Escape;
    glfw_tokey[GLFW_KEY_ENTER]         = Input::KeyInput::Enter;
    glfw_tokey[GLFW_KEY_SPACE]         = Input::KeyInput::Space;
    glfw_tokey[GLFW_KEY_TAB]           = Input::KeyInput::Tab;
    glfw_tokey[GLFW_KEY_BACKSPACE]     = Input::KeyInput::Backspace;
    glfw_tokey[GLFW_KEY_INSERT]        = Input::KeyInput::Insert;
    glfw_tokey[GLFW_KEY_DELETE]        = Input::KeyInput::Delete;
    glfw_tokey[GLFW_KEY_RIGHT]         = Input::KeyInput::Right;
    glfw_tokey[GLFW_KEY_LEFT]          = Input::KeyInput::Left;
    glfw_tokey[GLFW_KEY_DOWN]          = Input::KeyInput::Down;
    glfw_tokey[GLFW_KEY_UP]            = Input::KeyInput::Up;
    glfw_tokey[GLFW_KEY_PAGE_UP]       = Input::KeyInput::PageUp;
    glfw_tokey[GLFW_KEY_PAGE_DOWN]     = Input::KeyInput::PageDown;
    glfw_tokey[GLFW_KEY_HOME]          = Input::KeyInput::Home;
    glfw_tokey[GLFW_KEY_END]           = Input::KeyInput::End;
    glfw_tokey[GLFW_KEY_CAPS_LOCK]     = Input::KeyInput::CapsLock;
    glfw_tokey[GLFW_KEY_SCROLL_LOCK]   = Input::KeyInput::ScrollLock;
    glfw_tokey[GLFW_KEY_NUM_LOCK]      = Input::KeyInput::NumLock;
    glfw_tokey[GLFW_KEY_PRINT_SCREEN]  = Input::KeyInput::PrintScreen;
    glfw_tokey[GLFW_KEY_PAUSE]         = Input::KeyInput::Pause;

    glfw_tokey[GLFW_KEY_F1]  = Input::KeyInput::F1;
    glfw_tokey[GLFW_KEY_F2]  = Input::KeyInput::F2;
    glfw_tokey[GLFW_KEY_F3]  = Input::KeyInput::F3;
    glfw_tokey[GLFW_KEY_F4]  = Input::KeyInput::F4;
    glfw_tokey[GLFW_KEY_F5]  = Input::KeyInput::F5;
    glfw_tokey[GLFW_KEY_F6]  = Input::KeyInput::F6;
    glfw_tokey[GLFW_KEY_F7]  = Input::KeyInput::F7;
    glfw_tokey[GLFW_KEY_F8]  = Input::KeyInput::F8;
    glfw_tokey[GLFW_KEY_F9]  = Input::KeyInput::F9;
    glfw_tokey[GLFW_KEY_F10] = Input::KeyInput::F10;
    glfw_tokey[GLFW_KEY_F11] = Input::KeyInput::F11;
    glfw_tokey[GLFW_KEY_F12] = Input::KeyInput::F12;

    glfw_tokey[GLFW_KEY_LEFT_SHIFT]    = Input::KeyInput::LeftShift;
    glfw_tokey[GLFW_KEY_LEFT_CONTROL]  = Input::KeyInput::LeftControl;
    glfw_tokey[GLFW_KEY_LEFT_ALT]      = Input::KeyInput::LeftAlt;
    glfw_tokey[GLFW_KEY_LEFT_SUPER]    = Input::KeyInput::LeftSuper;
    glfw_tokey[GLFW_KEY_RIGHT_SHIFT]   = Input::KeyInput::RightShift;
    glfw_tokey[GLFW_KEY_RIGHT_CONTROL] = Input::KeyInput::RightControl;
    glfw_tokey[GLFW_KEY_RIGHT_ALT]     = Input::KeyInput::RightAlt;
    glfw_tokey[GLFW_KEY_RIGHT_SUPER]   = Input::KeyInput::RightSuper;

    glfw_tokey[GLFW_KEY_APOSTROPHE]    = Input::KeyInput::Apostrophe;
    glfw_tokey[GLFW_KEY_COMMA]         = Input::KeyInput::Comma;
    glfw_tokey[GLFW_KEY_MINUS]         = Input::KeyInput::Minus;
    glfw_tokey[GLFW_KEY_PERIOD]        = Input::KeyInput::Period;
    glfw_tokey[GLFW_KEY_SLASH]         = Input::KeyInput::Slash;
    glfw_tokey[GLFW_KEY_SEMICOLON]     = Input::KeyInput::Semicolon;
    glfw_tokey[GLFW_KEY_EQUAL]         = Input::KeyInput::Equal;
    glfw_tokey[GLFW_KEY_LEFT_BRACKET]  = Input::KeyInput::LeftBracket;
    glfw_tokey[GLFW_KEY_BACKSLASH]     = Input::KeyInput::Backslash;
    glfw_tokey[GLFW_KEY_RIGHT_BRACKET] = Input::KeyInput::RightBracket;
    glfw_tokey[GLFW_KEY_GRAVE_ACCENT]  = Input::KeyInput::GraveAccent;

    glfw_tokey[GLFW_KEY_KP_0]        = Input::KeyInput::Kp0;
    glfw_tokey[GLFW_KEY_KP_1]        = Input::KeyInput::Kp1;
    glfw_tokey[GLFW_KEY_KP_2]        = Input::KeyInput::Kp2;
    glfw_tokey[GLFW_KEY_KP_3]        = Input::KeyInput::Kp3;
    glfw_tokey[GLFW_KEY_KP_4]        = Input::KeyInput::Kp4;
    glfw_tokey[GLFW_KEY_KP_5]        = Input::KeyInput::Kp5;
    glfw_tokey[GLFW_KEY_KP_6]        = Input::KeyInput::Kp6;
    glfw_tokey[GLFW_KEY_KP_7]        = Input::KeyInput::Kp7;
    glfw_tokey[GLFW_KEY_KP_8]        = Input::KeyInput::Kp8;
    glfw_tokey[GLFW_KEY_KP_9]        = Input::KeyInput::Kp9;
    glfw_tokey[GLFW_KEY_KP_DECIMAL]  = Input::KeyInput::KpDecimal;
    glfw_tokey[GLFW_KEY_KP_DIVIDE]   = Input::KeyInput::KpDivide;
    glfw_tokey[GLFW_KEY_KP_MULTIPLY] = Input::KeyInput::KpMultiply;
    glfw_tokey[GLFW_KEY_KP_SUBTRACT] = Input::KeyInput::KpSubtract;
    glfw_tokey[GLFW_KEY_KP_ADD]      = Input::KeyInput::KpAdd;
    glfw_tokey[GLFW_KEY_KP_ENTER]    = Input::KeyInput::KpEnter;
    glfw_tokey[GLFW_KEY_KP_EQUAL]    = Input::KeyInput::KpEqual;

    glfw_tomouse[GLFW_MOUSE_BUTTON_LEFT]   = Input::MouseInput::Left;
    glfw_tomouse[GLFW_MOUSE_BUTTON_RIGHT]  = Input::MouseInput::Right;
    glfw_tomouse[GLFW_MOUSE_BUTTON_MIDDLE] = Input::MouseInput::Middle;
    glfw_tomouse[GLFW_MOUSE_BUTTON_4]      = Input::MouseInput::Button4;
    glfw_tomouse[GLFW_MOUSE_BUTTON_5]      = Input::MouseInput::Button5;
    glfw_tomouse[GLFW_MOUSE_BUTTON_6]      = Input::MouseInput::Button6;
    glfw_tomouse[GLFW_MOUSE_BUTTON_7]      = Input::MouseInput::Button7;
    glfw_tomouse[GLFW_MOUSE_BUTTON_8]      = Input::MouseInput::Button8;

    mapping_initalised = true;
}

static void key_callback(GLFWwindow* window, int glfwkey, int scancode, int action, int mods) {
    auto* userdata = static_cast<GlfwUserdata*>(glfwGetWindowUserPointer(window));
    if (!userdata || !userdata->inputs) return;

    if (glfwkey < 0 || glfwkey > GLFW_KEY_LAST) {
        return;
    }

    Input::KeyInput key = glfw_tokey[glfwkey];
    if (key == Input::KeyInput::Unknown) {
        return;
    }

    int index = static_cast<int>(key);
    userdata->inputs->SetKeyPressed(index, (action != GLFW_RELEASE));
}
static void mouse_button_callback(GLFWwindow* window, int glfwbutton, int action, int mods) {
    auto* userdata = static_cast<GlfwUserdata*>(glfwGetWindowUserPointer(window));
    if (!userdata || !userdata->inputs) return;

    if (glfwbutton < 0 || glfwbutton > GLFW_KEY_LAST) {
        return;
    }

    Input::MouseInput button = glfw_tomouse[glfwbutton];
    if (button == Input::MouseInput::Unknown) {
        return;
    }

    int index = static_cast<int>(button);
    userdata->inputs->SetMousePressed(index, (action != GLFW_RELEASE));
}
Input::Manager::Manager(GLFWwindow *ctx) : m_context(ctx) {
    init_input_maps();
    auto *userdata = static_cast<GlfwUserdata*>(glfwGetWindowUserPointer(m_context));
    userdata->inputs = this;
    glfwSetKeyCallback(m_context, key_callback);
    glfwSetMouseButtonCallback(m_context, mouse_button_callback);
}
bool Input::Manager::IsKeyDown(Input::KeyInput input) {
    return m_keys[static_cast<int>(input)].pressed;
}
bool Input::Manager::IsMouseButtonDown(Input::MouseInput input) {
    return m_mousebuttons[static_cast<int>(input)].pressed;
}
void Input::Manager::Update() {
    for (auto &inputinfo : m_keys) {
        inputinfo.was_pressed = inputinfo.pressed;
    }
    for (auto &inputinfo : m_mousebuttons) {
        inputinfo.was_pressed = inputinfo.pressed;
    }
    glfwPollEvents();
}
void Input::Manager::SetKeyPressed(int index, bool pressed) {
    m_keys[index].pressed = pressed;
}
void Input::Manager::SetMousePressed(int index, bool pressed) {
    m_mousebuttons[index].pressed = pressed;
}