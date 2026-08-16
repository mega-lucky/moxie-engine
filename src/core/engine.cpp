#include "./engine.hpp"

Engine::Engine() : MainWindow(800, 600, "New Window") {}
Engine::~Engine() {}

void Engine::Update() {
    WorldRegistry.Update();
    Timer.Update();
    Window::PollEvents();
    MainWindow.SwapBuffers();
    ScriptSchuduler.Resume(Timer.GetDeltaTime());
}