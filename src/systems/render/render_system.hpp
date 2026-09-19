#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include <engine.hpp>
#include <transform.h>

#include "./mesh.h"
#include "./shader.h"
#include "./types.h"
#include <text.h>

class RenderSystem : public World::IWorldSystem {
private:
    std::vector<draw_call> draw_calls;
    std::vector<view_proj> cam_matrices;
    World::Registry &world;
    Window::Container &window;

    ComponentID MeshRender;
    ComponentID MeshShape;
    ComponentID Transform;
    ComponentID Camera;
    ComponentID UiText;
public:
    RenderSystem(World::Registry &worldreg, Window::Container &window);
    void Update(double dt) override;
};

struct mesh_renderer {
    material_data *material;
};

#endif
