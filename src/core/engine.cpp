#include "./engine.hpp"

Engine::Engine(const engine_config &config) :
MainWindow(config.window_title, config.window_width, config.window_height, config.window_max),
ScriptSchuduler(*this),
InputManager(MainWindow.GetContext())
{}
Engine::~Engine() {}

void Engine::Update() {
    Timer.Update();
    double dt = Timer.GetDeltaTime();
    double elapsed = Timer.GetTime();
    InputManager.Update();
    WorldRegistry.Update(dt);
    ScriptSchuduler.Resume(elapsed);
    MainWindow.SwapBuffers();
}