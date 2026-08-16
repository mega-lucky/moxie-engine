#ifndef __ENGINE__H__
#define __ENGINE__H__

#include "./world.hpp"
#include "./script.hpp"
#include "./window.hpp"
#include "./time.hpp"

class Engine {
public:
    World::Registry WorldRegistry;
    Window::Container MainWindow;
    Script::Scheduler ScriptSchuduler;
    Time Timer;

    Engine();
    void Update();
    ~Engine();
};

#endif