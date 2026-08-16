#include "./engine.hpp"

Engine::Engine() : MainWindow(800, 600, "New Window") {}
Engine::~Engine() {}

void Engine::Update() {
    Timer.Update();
    double dt = Timer.GetDeltaTime();
    Window::PollEvents();
    WorldRegistry.Update(dt);
    ScriptSchuduler.Resume(dt);
    MainWindow.SwapBuffers();
}