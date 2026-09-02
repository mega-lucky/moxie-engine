#ifndef INPUT_MANAGER
#define INPUT_MANAGER

struct GLFWwindow;

namespace Input {

enum class KeyInput : int {
    Unknown = 0,
    
    A, B, C, D, E, F, G, H, I, J, K, L, M, 
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    
    Escape,
    Enter,
    Space,
    Tab,
    Backspace,
    Insert,
    Delete,
    Right, Left, Down, Up,
    PageUp, PageDown, Home, End,
    CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
    
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    LeftShift, LeftControl, LeftAlt, LeftSuper,
    
    RightShift, RightControl, RightAlt, RightSuper,
    
    Apostrophe,
    Comma,
    Minus,
    Period,
    Slash,
    Semicolon,   
    Equal,       
    LeftBracket, 
    Backslash,    
    RightBracket,
    GraveAccent,
    Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
    KpDecimal, KpDivide, KpMultiply, KpSubtract, KpAdd, KpEnter, KpEqual,

    Count
};

enum class MouseInput : int {
    Left = 0,
    Right,
    Middle,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,
    
    Count
};

struct InputInfo {
    bool pressed = false;
    bool was_pressed = false;
};

class Manager {
private:
    GLFWwindow *m_context;
    InputInfo m_keys[static_cast<int>(KeyInput::Count)];
    InputInfo m_mousebuttons[static_cast<int>(MouseInput::Count)];
public:
    Manager(GLFWwindow *);
    bool IsKeyDown(KeyInput);
    bool IsMouseButtonDown(MouseInput);
    void Update();

    void SetKeyPressed(int index, bool pressed);
    void SetMousePressed(int index, bool pressed);
};

}

#endif
