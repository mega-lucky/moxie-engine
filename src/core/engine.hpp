#ifndef __ENGINE__H__
#define __ENGINE__H__

#include "./world.hpp"
#include "./script.hpp"
#include "./window.hpp"
#include "./time.hpp"
#include "./input.hpp"

struct mesh_shape;
struct material_data;

struct AssetStore {
    std::unordered_map<std::string, mesh_shape*> meshes;
    std::unordered_map<std::string, material_data*> materials;
};

class Engine {
public:
    World::Registry WorldRegistry;
    Window::Container MainWindow;
    Script::Scheduler ScriptSchuduler;
    Input::Manager InputManager;
    Time Timer;
    AssetStore Assets;

    Engine();
    void Update();
    ~Engine();
};

#endif