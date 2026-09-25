#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include <memory>
#include <functional>

struct GLFWwindow;

namespace Window {
    
struct window_config {
    const char *title;
    int width, height;
    bool max;
    bool resizable;
    bool visible;
};

using ResizeCallback = std::function<void(int, int)>;

class Container {
private:
    int m_width, m_height;
    std::string m_name;
    GLFWwindow *m_context;
    std::vector<ResizeCallback> m_resize_callbacks;
public:
    Container(const window_config &config);
    ~Container();
    
    bool ShouldClose() const noexcept;
    int Width() const noexcept;
    int Height() const noexcept;
    double AspectRatio() const noexcept;
    void SwapBuffers();
    void Resize(int width, int height) noexcept;
    void BindResizeCallback(const ResizeCallback &cb);
    void Show();
    void Hide();
    GLFWwindow *GetContext();
};

void PollEvents();

}

#endif