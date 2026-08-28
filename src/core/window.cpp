#include "./window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

using WindowContext = GLFWwindow;

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

struct ResizeCallbackEntry {
    Window::ResizeCallback callback;
    ResizeCallbackEntry *next;
    int id;
};

static int glfw_ref_count = 0;

static void err_callback(int code, const char *desc) {
    (void)code;
    std::string message = "GLFW ERROR: " + std::string(desc);
    throw std::runtime_error(message);
}

static void resize_callback(WindowContext* context, int width, int height) {
    auto *window = static_cast<Window::Container*>(glfwGetWindowUserPointer(context));

    if (window == nullptr) {
        return;
    }

    window->Resize(width, height);
}

Window::Container::Container(int width, int height, const std::string name) :
    m_width(width),
    m_height(height),
    m_name(name)
{
    glfwInit();
    glfwSetErrorCallback(err_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfw_ref_count ++;

    m_context = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);

    if (m_context == nullptr) {
        throw std::runtime_error("Failed to initialise GLFW context.");
    }

    glfwMakeContextCurrent(m_context);
    glfwSetFramebufferSizeCallback(m_context, (GLFWframebuffersizefun)resize_callback);
    glfwSetWindowUserPointer(m_context, this);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
Window::Container::~Container() {
    glfwDestroyWindow(m_context);
    glfw_ref_count = std::max(glfw_ref_count - 1, 0);

    if (glfw_ref_count == 0) {
        glEnd();
        glfwTerminate();
    }
}
bool Window::Container::ShouldClose() const noexcept {
    return glfwWindowShouldClose(m_context);
}
int Window::Container::Width() const noexcept {
    return m_width;
}
int Window::Container::Height() const noexcept {
    return m_height;
}
double Window::Container::AspectRatio() const noexcept {
    return m_height != 0 ?
        static_cast<double>(m_width) / static_cast<double>(m_height) :
        0;
}
void Window::Container::SwapBuffers() {
    if (glfwGetCurrentContext() != m_context) {
        glfwMakeContextCurrent(m_context);
    }
    glfwSwapBuffers(m_context);
}
void Window::Container::Resize(int width, int height) noexcept {
    m_width = width;
    m_height = height;

    for (auto &cb : m_resize_callbacks) {
        cb(width, height);
    }
}
void Window::Container::BindResizeCallback(const ResizeCallback &cb) {
    m_resize_callbacks.push_back(cb);
}
void Window::Container::Show() {
    glfwSetWindowTitle(m_context, m_name.c_str());
    glfwSetWindowSize(m_context, m_width, m_height);
    glfwShowWindow(m_context);
}
void Window::Container::Hide() {
    glfwHideWindow(m_context);
}
void Window::PollEvents() {
    glfwPollEvents();
}
