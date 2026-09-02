#ifndef INPUT_MANAGER
#define INPUT_MANAGER

struct GLFWwindow;

#define KEY_BUTTONS_MACRO \
    X(A) X(B) X(C) X(D) X(E) X(F) X(G) X(H) X(I) X(J) X(K) X(L) X(M) \
    X(N) X(O) X(P) X(Q) X(R) X(S) X(T) X(U) X(V) X(W) X(X) X(Y) X(Z) \
    X(Num0) X(Num1) X(Num2) X(Num3) X(Num4) X(Num5) X(Num6) X(Num7) X(Num8) X(Num9) \
    X(Escape) X(Enter) X(Space) X(Tab) X(Backspace) X(Insert) X(Delete) \
    X(Right) X(Left) X(Down) X(Up) X(PageUp) X(PageDown) X(Home) X(End) \
    X(CapsLock) X(ScrollLock) X(NumLock) X(PrintScreen) X(Pause) \
    X(F1) X(F2) X(F3) X(F4) X(F5) X(F6) X(F7) X(F8) X(F9) X(F10) X(F11) X(F12) \
    X(LeftShift) X(LeftControl) X(LeftAlt) X(LeftSuper) \
    X(RightShift) X(RightControl) X(RightAlt) X(RightSuper) \
    X(Apostrophe) X(Comma) X(Minus) X(Period) X(Slash) X(Semicolon) \
    X(Equal) X(LeftBracket) X(Backslash) X(RightBracket) X(GraveAccent) \
    X(Kp0) X(Kp1) X(Kp2) X(Kp3) X(Kp4) X(Kp5) X(Kp6) X(Kp7) X(Kp8) X(Kp9) \
    X(KpDecimal) X(KpDivide) X(KpMultiply) X(KpSubtract) X(KpAdd) X(KpEnter) X(KpEqual)

#define MOUSE_BUTTONS_MACRO \
    X(Left) \
    X(Right) \
    X(Middle) \
    X(Button4) \
    X(Button5) \
    X(Button6) \
    X(Button7) \
    X(Button8)

namespace Input {

enum class KeyInput : int {
    Unknown = 0,

    #define X(n) n,
    KEY_BUTTONS_MACRO
    #undef X

    Count
};

enum class MouseInput : int {
    Unknown = 0,
    
    #define X(n) n,
    MOUSE_BUTTONS_MACRO
    #undef X
    
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
