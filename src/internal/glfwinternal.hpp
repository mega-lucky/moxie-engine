#ifndef GLFW_INTERNAL
#define GLFW_INTERNAL

namespace Window { class Container; }
namespace Input { class Manager; }

struct GlfwUserdata {
    Window::Container *window;
    Input::Manager *inputs;
};

#endif