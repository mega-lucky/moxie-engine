#include "moxie/window.h"
#include "moxie/inputs.h"
#include "moxlib/memory.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <signal.h>

struct ResizeCallbackEntry {
    WindowResizeCallback callback;
    struct ResizeCallbackEntry *next;
    int id;
};
typedef struct ResizeCallbackEntry ResizeCallbackEntry;

#define DEFAULT_WINDOW_NAME "This is my window"
#define DEFAULT_WINDOW_SIZE 800, 600

static ResizeCallbackEntry *resize_callbacks = NULL;

static void err_callback(int code, const char *desc) {
    (void)code;
    printf("GLFW ERROR:\n%s\n", desc);
}

static int ScreenWidth, ScreenHeight;

static void resize_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    ScreenWidth = width;
    ScreenHeight = height;
    
    ResizeCallbackEntry *ptr = resize_callbacks;
    while (ptr != NULL) {
        ptr->callback(width, height);
        ptr = ptr->next;
    }
}

void handle_sigint(int sig) {
    (void)sig;
    glfwSetWindowShouldClose(glfwGetCurrentContext(), 1);
}

int InitWindow() {
    glfwInit();
    glfwSetErrorCallback(err_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_NAME, NULL, NULL);

    if (window == NULL) { printf("FAILED TO CREATE WINDOW\n"); return 0; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)resize_callback);
    BindInputCallbacks(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int win_width, win_height;
    glfwGetWindowSize(window, &win_width, &win_height);
    resize_callback(window, win_width, win_height);

    signal(SIGINT, handle_sigint);

    return 1;
}

int ShouldWindowClose() {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

int GetWindowWidth() {
    return ScreenWidth;
}

int GetWindowHeight() {
    return ScreenHeight;
}

float GetWindowAspectRatio() {
    return (float)ScreenWidth / (float)ScreenHeight;
}

void TerminateWindow() {
    glEnd();
    glfwTerminate();
}

void SwapFrameBuffers() {
    GLFWwindow *current = glfwGetCurrentContext();
    if (current == NULL) { return; }

    glfwSwapBuffers(current);
}

double GetTime() {
    return glfwGetTime();
}

void BindToWindowResize(WindowResizeCallback cb) {
    ResizeCallbackEntry **ptr = &resize_callbacks;
    while (*ptr) {
        ptr = &(*ptr)->next;
    }
    ResizeCallbackEntry *new = AllocMem(sizeof(ResizeCallbackEntry));
    new->callback = cb;
    new->id = 0;
    new->next = NULL;

    *ptr = new;
}