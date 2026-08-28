#include "./engine.hpp"

Engine::Engine() : MainWindow(800, 600, "New Window"), ScriptSchuduler(*this) {}
Engine::~Engine() {}

void Engine::Update() {
    Timer.Update();
    double dt = Timer.GetDeltaTime();
    double elapsed = Timer.GetTime();
    Window::PollEvents();
    WorldRegistry.Update(dt);
    ScriptSchuduler.Resume(elapsed);
    MainWindow.SwapBuffers();
}