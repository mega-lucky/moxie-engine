#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include <engine.hpp>
#include <transform.hpp>

#include "./mesh.h"
#include "./shader.h"
#include "./types.h"

class RenderSystem : public World::IWorldSystem {
private:
    std::vector<draw_call> draw_calls;
    World::Registry &world;

    ComponentID MeshRender;
    ComponentID MeshShape;
    ComponentID Transform;
public:
    RenderSystem(Engine &e);
    void Update(double dt) override;
};

struct mesh_renderer {
    material_data *material;
};

#endif
